import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.TreeMap;

public class Model {

	private int forward;
	private int right;
	private int left;
	private String winningDirection;
	
	public Model() {
		this.forward = 0;
		this.right = 0;
		this.left = 0;
		this.winningDirection = "Nothing";
	}
	
	public void updateForward() {
		this.forward++;
	}
	
	public void updateRight() {
		this.right++;
	}
	
	public void updateLeft() {
		this.left++;
	}
	
	public int getFordward() {
		return this.forward;
	}
	
	public int getRight() {
		return this.right;
	}
	
	public int getLeft() {
		return this.left;
	}
	
	public String getWinningDirection() {
		TreeMap<Integer, String> mapping = new TreeMap<Integer, String>();
		mapping.put(this.forward, "Forward");
		mapping.put(this.right, "Right");
		mapping.put(this.left, "Left");
		Integer highestVote = mapping.lastKey();
		this.winningDirection = mapping.get(highestVote); 
		return this.winningDirection;
	}
	
	public String simpleWinningDirection() {
		return this.winningDirection;
	}
	
	public void reset() {
		this.forward = 0;
		this.right = 0;
		this.left = 0;
	}
}
