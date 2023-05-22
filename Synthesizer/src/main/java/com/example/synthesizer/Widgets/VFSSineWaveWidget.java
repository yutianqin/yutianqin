package com.example.synthesizer.Widgets;

import com.example.synthesizer.AudioComponent;
import com.example.synthesizer.VFSineWave;
import com.example.synthesizer.linearRamp;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;



public class VFSSineWaveWidget extends AudioComponentWidget {

    linearRamp ramp = new linearRamp(500, 2000);

    public VFSSineWaveWidget( AnchorPane parent, String name ) {

        super( parent, name );

        AudioComponent ac = new VFSineWave();
        ac.connectInput( ramp  );

        audioComponent_ = ac;

        parent_ = parent;

        name_ = name;

//        baseLayout = new HBox();

        baseLayout.setStyle("-fx-border-color: black; " +
                "-fx-border-image-width: 10");

//        whoIamSendingMyOutputTo = sendto;



        // Right Side:
        // Close Btn, Connect Point
        VBox rightSide = new VBox();    // Child of HBox baseLayout
        rightSide.setStyle("-fx-background-color: orange");

        Button closeButton = new Button("x");   // Close Button
        closeButton.setOnAction( e -> closeWidget() );

        Circle circle = new Circle( 10 );   // Connect Point
        circle.setFill( Color.GRAY );
        circle.setOnMousePressed( e -> startConnection( e, circle ) );
        circle.setOnMouseDragged( e -> moveConnection(e, circle1) );
        circle.setOnMouseReleased( e -> endConnection(e, circle2) );

        rightSide.getChildren().add( closeButton );
        rightSide.getChildren().add( circle );
        rightSide.setAlignment( Pos.CENTER ); // Set all the child nodes' positions at the center
        rightSide.setPadding( new Insets( 5 ) );
        rightSide.setSpacing( 5 ); // Space between two elements




        // Left Side:
        VBox leftSide = new VBox();    // Child of HBox baseLayout
        leftSide.setStyle("-fx-background-color: orange");

        circle2 = new Circle( 10 );   // Connect Point
        circle2.setFill( Color.BLACK );
        circle2.setOnMousePressed( e -> startConnection( e, circle2 ) );
        circle2.setOnMouseDragged( e -> moveConnection(e, circle1) );
        circle2.setOnMouseReleased( e -> endConnection(e, circle2) );

        leftSide.getChildren().add( circle );
        leftSide.setAlignment( Pos.CENTER ); // Set all the child nodes' positions at the center
        leftSide.setPadding( new Insets( 5 ) );
        leftSide.setSpacing( 5 ); // Space between two elements




        // Middle
        // Label, Slider
        VBox componentWidget = new VBox();  // Child of HBox baseLayout
        componentWidget.setStyle("-fx-background-color: lightgrey");
        componentWidget.setAlignment( Pos.CENTER );
        componentWidget.setPadding( new Insets(5) );
        componentWidget.setSpacing( 5 );

        Slider low = new Slider( 0, 440, 220 );
        low.setOnMouseDragged( e -> Sliderlow( e, low, nameLable_));
        Slider high = new Slider( 440, 880, 660);
        high.setOnMouseDragged( e -> Sliderhigh( e, high, nameLable_));

        nameLable_ = new Label();// Used as the title of Vbox(playStopWidget)
        nameLable_.setMouseTransparent( true );
        nameLable_.setText( name_ + " (440 Hz)" );


        componentWidget.getChildren().add( low );
        componentWidget.getChildren().add( high );

//        componentWidget.relocate( 100, 200 );
        componentWidget.setOnMousePressed( e -> startDrag( e ) );
        componentWidget.setOnMouseDragged( e -> handleDrag( e, circle, line_) );


        baseLayout.getChildren().add( leftSide );
        baseLayout.getChildren().add( componentWidget );
        baseLayout.getChildren().add( rightSide );



        this.getChildren().add( baseLayout );
        this.setLayoutX( 50 );
        this.setLayoutY( 100 );
        parent_.getChildren().add( this );
    }

    private void Sliderhigh(MouseEvent e, Slider high, Label nameLable_) {
        int value = (int) high.getValue();
        nameLable_.setText( name_ + " (" + value + " Hz)" );
    }

    private void Sliderlow(MouseEvent e, Slider low, Label nameLable_) {
        int value = (int) low.getValue();
        nameLable_.setText( name_ + " (" + value + " Hz)" );
    }



//    private void startDrag(MouseEvent e) {
//        // Cursor's Initial Position
//        mouseStartDragX = e.getSceneX();
//        mouseStartDragY = e.getSceneY();
//        // Widget's Initial Position
//        widgetStartDragX = this.getLayoutX();
//        widgetStartDragY = this.getLayoutY();
//    }
//
//    private void handleDrag(MouseEvent e) {
//        // New Position minus initial position
//        double mouseDelX = e.getSceneX() - mouseStartDragX;
//        double mouseDelY = e.getSceneY() - mouseStartDragY;
//        // Difference of new Pos and initial Pos + Widget's initial Pos
//        this.relocate( widgetStartDragX + mouseDelX , widgetStartDragY + mouseDelY );
//    }
}
