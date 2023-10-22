use tiny_http::{Response, Server};

#[tokio::main]
async fn main() -> std::io::Result<()> {
    println!("Hello, Buried!");
    use tiny_http::{Response, Server};

    let server = Server::http("0.0.0.0:5678").unwrap();

    for mut request in server.incoming_requests() {
        println!(
            "received request! method: {:?}, url: {:?}, headers: {:?}",
            request.method(),
            request.url(),
            request.headers()
        );

        match request.method() {
            tiny_http::Method::Post => {
                let mut content = String::new();
                request.as_reader().read_to_string(&mut content).unwrap();
                println!("content: {}", content);
            }

            _ => {
                println!("not a post request");
            }
        }

        let response = Response::from_string("{\"code\": 0}");
        let _ = request.respond(response);
    }

    Ok(())
}
