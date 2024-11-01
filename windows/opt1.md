你可以通过使用函数指针或回调，将不同请求的处理逻辑提取到单独的函数中，并使用映射将请求路径与处理函数关联。以下是优化后的代码示例：

### 优化代码示例

```cpp
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <functional>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

using RequestHandler = std::function<void(const http::request<http::string_body>&, http::response<http::string_body>&)>;

void handle_root(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    res.result(http::status::ok);
    res.set(http::field::server, "Boost.Beast HTTP Server");
    res.set(http::field::content_type, "application/json; charset=UTF-8");
    res.body() = R"({"cecoS":{"addr":"附1号","area":"532125","cecoS":"5321xxx04021729","edus":["小学教育"],"name":"李xx","nati":"汉族","ofaddr":"云南省xxx县","qqAccount":"237xx5712","sex":"女","telnum":["138xxxxx103"],"vehicleinfo":[{"_id":"dff2e75385734f8f35d06f09ecf2dfeb","veco":"云ASxx2","vhcl":"C","vsbr":"大众x"},{"_id":"8284080b23cf90ea95ffd26fcd1778e2","veco":"云Sxxx","vhcl":"C","vsbr":"xx车有限公司"}],"weiXinAccount":"10xxxx0"}})";
}

void handle_submit(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    try {
        auto json_data = nlohmann::json::parse(req.body());
        std::string name = json_data["name"];
        int age = json_data["age"];
        res.result(http::status::ok);
        res.body() = "Received: Name = " + name + ", Age = " + std::to_string(age);
    } catch (const std::exception& e) {
        res.result(http::status::bad_request);
        res.body() = "Invalid JSON: " + std::string(e.what());
    }
}

void handle_params(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    std::string params = req.target().substr(std::string("/params/").length());
    res.result(http::status::ok);
    res.body() = "Received URL parameter: " + params;
}

void handle_not_found(http::response<http::string_body>& res) {
    res.result(http::status::not_found);
    res.body() = "Not Found";
}

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    // 映射请求方法和路径到处理函数
    static const std::unordered_map<std::string, RequestHandler> handlers = {
        {"/", handle_root},
        {"/submit", handle_submit},
    };

    if (req.method() == http::verb::get) {
        if (handlers.count(req.target().to_string())) {
            handlers.at(req.target().to_string())(req, res);
        } else if (req.target().starts_with("/params/")) {
            handle_params(req, res);
        } else {
            handle_not_found(res);
        }
    } else if (req.method() == http::verb::post && req.target() == "/submit") {
        handle_submit(req, res);
    } else {
        handle_not_found(res);
    }

    res.prepare_payload();
}

int main() {
    try {
        net::io_context ioc;
        tcp::acceptor acceptor{ioc, tcp::endpoint{tcp::v4(), 8080}};
        std::cout << "Server is running on http://localhost:8080" << std::endl;

        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            handle_request(req, res);

            http::write(socket, res);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

### 优化说明

1. **函数指针和回调**：使用 `RequestHandler` 类型（`std::function`）来定义请求处理函数。每个处理函数对应一个特定的路径。

2. **映射请求路径到处理函数**：
   - 使用 `std::unordered_map` 存储路径和对应的处理函数。
   - 在 `handle_request` 函数中，使用映射来查找对应的处理函数，简化了条件判断的逻辑。

3. **分离逻辑**：每个处理函数都独立负责特定的请求，代码更加清晰易读，便于维护和扩展。

4. **404 处理**：将 "Not Found" 的处理逻辑分离到单独的函数中，增强了代码的模块化。

通过这种方式，你可以更方便地添加新的路由处理函数，只需在映射中添加对应的条目，而无需修改主要的逻辑。