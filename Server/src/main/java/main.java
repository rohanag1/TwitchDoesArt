import static spark.Spark.port;
import static spark.Spark.post;
import static spark.Spark.get;
import static spark.Spark.staticFileLocation;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import spark.ModelAndView;
import spark.TemplateEngine;
import spark.template.velocity.VelocityTemplateEngine;

public class main {
	private static ReentrantReadWriteLock rwl;
	private static Model model = new Model();
	public static void main(String[] args) {
		
		staticFileLocation("/public");
	    String layout = "templates/layout.vtl";
	    port(90);
	    get("/direction", (request, response) -> {
	      Map<String, Object> model = new HashMap<String, Object>();
	      model.put("template", "templates/direction.vtl" );
	      return new ModelAndView(model, layout);
	    }, new VelocityTemplateEngine());

	    get("/logData", (request, response) -> {
	    	if(request.queryParams("direction").contentEquals("forward")) model.updateForward(); 
	    	if(request.queryParams("direction").contentEquals("right")) model.updateRight(); 
	    	if(request.queryParams("direction").contentEquals("left")) model.updateLeft(); 
	    	Map<String, Object> modelNew = new HashMap<String, Object>();
	    	modelNew.put("template", "templates/direction.vtl" );
	    	modelNew.put("winningDirection", model.simpleWinningDirection());
		    return new ModelAndView(modelNew, layout);
	    }, new VelocityTemplateEngine());

	    get("/getData", (request, response) -> {
	    	response.header("Content-Type", "text/plain");
	    	response.status(201);
	    	String returnDirection = model.getWinningDirection();
	    	model.reset();
	    	return returnDirection;
	    
	    });
	    
	}

}
