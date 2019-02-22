import static spark.Spark.port;
import static spark.Spark.get;
import org.json.*;

import com.google.gson.Gson;

public class Server {
	/** The single instance of the server. */
	private static Server serverInstance;
	/** The port on which the server is run. */
	private static int portNumber;
	/** The password for read access to the server. */
	
	private Server(int portNum) {
		portNumber = portNum;
		
	}

	public static Server getInstance(int portNum) {
		serverInstance = new Server(portNum);
		return serverInstance;
		
	}
	
	public void run() {
		Gson gson = new Gson();
		port(portNumber);
		get("/login", (request, response) -> {
			response.header("Content-Type", "application/json");
			JSONObject responseValue = new JSONObject();
			responseValue.put("session_id", new Integer(1));
			response.status(200);
			return responseValue;
				
		});
	}
}

