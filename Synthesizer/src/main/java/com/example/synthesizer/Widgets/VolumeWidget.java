package com.example.synthesizer.Widgets;

import com.example.synthesizer.AudioComponent;
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

public class VolumeWidget extends AudioComponentWidget{

    int value;


    public VolumeWidget(AudioComponent ac, AnchorPane parent, String name) {

        super(ac, parent, name );

        audioComponent_ = ac;

        name_ = name;

//        baseLayout = new HBox();

        baseLayout.setStyle("-fx-border-color: black; " + "-fx-border-image-width: 10");

//        whoIamSendingMyOutputTo = null;


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
        rightSide.setPadding( new Insets( 5 ) ); // Set space around elements
        rightSide.setSpacing( 5 ); // Space between two elements





        // Left Side:
        // Label, Slider
        VBox componentWidget = new VBox();  // Child of HBox baseLayout
        componentWidget.setStyle("-fx-background-color: lightgrey");
        componentWidget.setAlignment( Pos.CENTER );
        componentWidget.setPadding( new Insets(5) );
        componentWidget.setSpacing( 5 );

        nameLable_ = new Label();// Used as the title of Vbox(playStopWidget)
        nameLable_.setMouseTransparent( true );
        nameLable_.setText( name_ +" 50" );
        Slider slider = new Slider( 0,100,50 );
        slider.setOnMouseDragged( e -> handleVol( e, slider, nameLable_));

        componentWidget.getChildren().add( nameLable_ );
        componentWidget.getChildren().add( slider );

//        componentWidget.relocate( 100, 200 );
        componentWidget.setOnMousePressed( e -> startDrag( e ) );
        componentWidget.setOnMouseDragged( e -> handleDrag( e , circle, line_) );

        baseLayout.getChildren().add ( componentWidget );
        baseLayout.getChildren().add( rightSide );

        this.getChildren().add( baseLayout );
        this.setLayoutX( 50 );
        this.setLayoutY( 100 );
//        parent.getChildren().add( this );
    }

    void handleVol(MouseEvent e, Slider slider, Label title) {
        value = (int) slider.getValue();
        title.setText( name_ + " (" + value + ")" );
    }
}
