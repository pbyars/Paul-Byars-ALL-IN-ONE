/*	
 *  Name: Paul Byars
 * 	Class: CIS-315-I01
 * 	Instructor: Xihui "Paul" Zhang
 * 	Date: 07/15/2020
 * 	Description: This program creates a series of "buckets" that allow for a series of 
 * 				 integers to be added to them. The program will use a sorting algorithm 
 * 				 to sort pre-supplied values to be added to the buckets. Once the values 
 * 				 have been sorted, the program will indicate that they have been sorted. 
 */

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.Scene;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class RadixSortAnimation extends Application {
    double radius = 2;
    private java.util.ArrayList<Integer>[] bucket = new java.util.ArrayList[10];
    private int[] list = new int[ARRAY_SIZE];
    private int position = 0;
    private int num = 0;
    private int key;
    public final static int ARRAY_SIZE = 20;

    public void createList() {
        for (int i = 0; i < list.length; i++) {
            list[i] = (int)(Math.random() * 1000);
        }
    }

    public void createBuckets() {
        for (int i = 0; i < bucket.length; i++) {
            bucket[i] = new java.util.ArrayList<>();
        }
    }

    public void deleteBuckets() {
        for (int i = 0; i < bucket.length; i++) {
            bucket[i].clear();
        }
    }

    public void resetBtn() {
        deleteBuckets();
        num = 0;
        position = 0;
        createList();
    }

    public void buttonWithList() {
        int value = 0;
        for (int i = 0; i < bucket.length; i++) {
            for (int j = 0; j < bucket[i].size(); j++) {
                list[value++] = bucket[i].get(j);
            }
        }
    }

    public boolean stepper() {
        if (num < list.length) {
            key = getKey(list[num], position);
            bucket[key].add(list[num]);
            
            num++;
            
            return false;
        } 
        else if (position < 2) {
        	buttonWithList();
            deleteBuckets();
            
            num = 0;
            position++;
            
            return false;
        } 
        else {
        	buttonWithList();
            deleteBuckets();
            
            return true;
        }
    }

    public static int getKey(int theNumber, int thePosition) {
        int result = 1;
        
        for (int i = 0; i < thePosition; i++) {
            result *= 10;
        }

        return (theNumber / result) % 10;
    }

    @Override
    public void start(Stage primaryStage) {
        AnimationPane pane = new AnimationPane();
        pane.setStyle("-fx-border-color: black");

        Button buttonOne = new Button("Step");
        Button buttonTwo = new Button("Reset");

        HBox hBox = new HBox(5);
        hBox.getChildren().addAll(buttonOne, buttonTwo);
        hBox.setAlignment(Pos.CENTER);

        BorderPane borderPane = new BorderPane();
        borderPane.setCenter(pane);
        borderPane.setBottom(hBox);

        Label label = new Label();
        borderPane.setTop(label);
        BorderPane.setAlignment(label, Pos.CENTER);

        Scene scene = new Scene(borderPane, 650, 300);
        primaryStage.setTitle("Radix Sort Animation");
        
        primaryStage.setScene(scene);
        primaryStage.show();

        createList();
        createBuckets();
        pane.repaint();

        buttonOne.setOnAction(e -> {
            if (stepper()) {
                pane.repaint();
                label.setText("The array is already sorted!");
            } 
            else {
                pane.repaint();
            }
        });

        buttonTwo.setOnAction(e -> {
            resetBtn();
            
            label.setText("");
            pane.repaint();
        });
    }
    
    class AnimationPane extends Pane {
        private int startingPosOne = 20;
        private int startingPosTwo = 20;
        private int bucketWidth = 30;
        private int bucketHeight = 20;

        protected void repaint() {
            this.getChildren().clear();

            int one = startingPosOne + 10;
            int two = startingPosTwo + 10;

            for (int i = 0; i < list.length; i++) {
                Rectangle rectangle = new Rectangle(one, two, bucketWidth, bucketHeight);
                
                rectangle.setFill(Color.WHITE);
                rectangle.setStroke(Color.BLACK);
                this.getChildren().add(rectangle);
                
                Text text = new Text(one + 5, two + 15, list[i] + "");
                
                this.getChildren().add(text);
                
                if (num - 1 == i) {
                    text.setFill(Color.RED);
                } 
                else {
                    text.setFill(Color.BLACK);
                }

                one += bucketWidth;
            }

            one = startingPosOne + 10;
            two += 40;
            
            int bucketWidth = 40;
            int bucketHeight = 130;
            int bucketDistance = 60;
            
            for (int i = 0; i < bucket.length; i++) {
                Rectangle rectangle = new Rectangle(one + 10, two, bucketWidth, bucketHeight);
                
                rectangle.setFill(Color.WHITE);
                rectangle.setStroke(Color.BLACK);
                this.getChildren().add(rectangle);

                for (int j = 0; j < bucket[i].size(); j++) {
                    this.getChildren().add(new Text(one + 10 + 5, two + (j + 1) * 20, bucket[i].get(j) + ""));
                }

                if (i == key && bucket[i].size() > 0) {
                    Text text = new Text(one + 10 + 5, two + bucket[i].size() * 20, bucket[i].get(bucket[i].size() - 1) + "");
                    
                    text.setFill(Color.RED);
                    this.getChildren().add(text);
                }

                Text text = new Text(one + 5, two + bucketHeight + 25, "bucket[" + i + "]");
                
                getChildren().add(text);
                one += bucketDistance;
            }
        }
    }

    public static void main(String[] args) {
        Application.launch(args);
    }
}