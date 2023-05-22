package com.example.synthesizer.Widgets;
import com.example.synthesizer.AudioComponent;
import com.example.synthesizer.SineWave;
import com.example.synthesizer.Square;
import com.example.synthesizer.SynthesizeApplication;
import javafx.geometry.Bounds;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.control.Button;

import java.util.ArrayList;

public class AudioComponentWidget extends Pane {



    AudioComponent audioComponent_;
    AnchorPane parent_;
//    HBox baseLayout;
    BorderPane baseLayout;
    String name_;
    Label nameLable_;
    Line line_;

    public Slider slider;

    double mouseStartDragX, mouseStartDragY, widgetStartDragX, widgetStartDragY;

    AudioComponentWidget widgetIamSendingOutputTo_ = null;


    Circle circle1;
    Circle circle2;



    public AudioComponentWidget(AnchorPane parent, String name) {}
    public AudioComponentWidget(AudioComponent ac, AnchorPane parent, String name ) //fourth para newly added
    {
        audioComponent_ = ac;

        parent_ = parent;

        name_ = name;

        baseLayout = new BorderPane();

        baseLayout.setStyle("-fx-border-color: black; " + "-fx-border-image-width: 10");

        widgetIamSendingOutputTo_ = null;

        // Right Side:
        // Close Btn, Connect Point
        VBox rightSide = new VBox();    // Child of HBox baseLayout
        rightSide.setStyle("-fx-background-color: orange");

        Button closeButton = new Button("x");   // Close Button
        closeButton.setOnAction( e -> closeWidget() );

        circle1 = new Circle( 10 );   // Connect Point
        circle1.setFill( Color.LIGHTPINK );
        circle1.setOnMousePressed( e -> startConnection( e, circle1 ) );
        circle1.setOnMouseDragged( e -> moveConnection(e, circle1) );
        circle1.setOnMouseReleased( e -> endConnection(e, circle2) );
        rightSide.getChildren().add( closeButton );
        rightSide.getChildren().add( circle1 );
        rightSide.setAlignment( Pos.CENTER );
        rightSide.setPadding( new Insets( 5 ) );
        rightSide.setSpacing( 5 );
        baseLayout.setRight( rightSide );



//        // Left Side:
//        VBox leftSide = new VBox();    // Child of HBox baseLayout
//        leftSide.setStyle("-fx-background-color: grey");
//        circle2 = new Circle( 10 );   // Connect Point
//        circle2.setFill( Color.LIGHTGRAY );
//        circle2.setOnMousePressed( e -> startConnection( e, circle2 ) );
//        circle2.setOnMouseDragged( e -> moveConnection(e, circle2 ) );
//        circle2.setOnMouseReleased( e -> endConnection(e, circle2 ) );
//        leftSide.setAlignment( Pos.CENTER );
//        leftSide.getChildren().add( circle2 );
//        leftSide.setPadding( new Insets( 5 ) );
//        leftSide.setSpacing( 5 );
//        baseLayout.setLeft( leftSide );


        //Middle
        // Label, Slider
        VBox center = new VBox();  // Child of HBox baseLayout
        center.setStyle("-fx-background-color: lightgrey");
        center.setAlignment( Pos.CENTER );
        center.setPadding( new Insets(5) );
        center.setSpacing( 5 );
        nameLable_ = new Label();// Used as the title of Vbox(playStopWidget)
        nameLable_.setMouseTransparent( true );
        nameLable_.setText( name_ + " (440 Hz)" );
        slider = new Slider( 220,880,440 );
        slider.setOnMouseDragged( e -> handleSlider( e, slider, nameLable_));
        center.getChildren().add( nameLable_ );
        center.getChildren().add( slider );
        center.setOnMousePressed( e -> startDrag( e ) );
        center.setOnMouseDragged( e -> handleDrag( e, circle1, line_) );
        baseLayout.setCenter( center );





        this.getChildren().add( baseLayout );
        this.setLayoutX( 50 );
        this.setLayoutY( 100 );
        parent_.getChildren().add( this );
    }



    void endConnection(MouseEvent e, Circle circle) {

        ArrayList<Bounds> boundsList = new ArrayList<>();
        ArrayList<Double> inputDistance = new ArrayList<>();
        Circle speaker = SynthesizeApplication.speaker_;
        Circle input;

        Bounds anchorBound = parent_.getBoundsInParent();
        Bounds speakerBounds = speaker.localToScreen(speaker.getBoundsInLocal());

        for ( AudioComponentWidget w: SynthesizeApplication.allWidgets ) {
            input = circle;
            if (input != null) {
                Bounds b = input.localToScene(input.getBoundsInLocal());
                boundsList.add(b);
            }
        }

        double speakerDistance = Math.sqrt( Math.pow( speakerBounds.getCenterX() - e.getScreenX(), 2.0 )
                                   + Math.pow( speakerBounds.getCenterY() - e.getScreenY(), 2.0 ) );

        for ( Bounds b : boundsList ) {
            if (b != null) {
                double inDist = Math.sqrt((Math.pow((b.getCenterX() - anchorBound.getMinX()) - e.getSceneX(), 2)) +
                        (Math.pow((b.getCenterY() - anchorBound.getMinY()) - e.getSceneY(), 2)));
                inputDistance.add(inDist);
            }
        }

        if (inputDistance.size() > 0) {
            for (double d : inputDistance) {
                if (d < 20) {
                    SynthesizeApplication.allWidgets.add(this);
                } else if (speakerDistance < speaker.getRadius()) {
                    SynthesizeApplication.allWidgets.add(this);
                } else {
                    parent_.getChildren().remove(line_);
                    line_ = null;
                }
            }
        } else {
            if (speakerDistance < speaker.getRadius()) {
                SynthesizeApplication.allWidgets.add(this);
            } else {
                parent_.getChildren().remove(line_);
                line_ = null;
            }
        }

//        if ( distance < 15 ) {
//            SynthesizeApplication.connectedWg.add( this );
//        }
//        else {
//            parent_.getChildren().remove( line_ );
//            line_ = null;
//        }
    }


//    public double getSliderValue( ) {
//        return slider.getValue();
//    }


    void moveConnection(MouseEvent e, Circle circle1) {
        Bounds parentBounds = parent_.getBoundsInParent();
        line_.setEndX( e.getSceneX() - parentBounds.getMinX() );
        line_.setEndY( e.getSceneY() - parentBounds.getMinY() );
    }

    void startConnection(MouseEvent e, Circle circle) {
        // If a line exists (in other words, if we are connected to someone else
        if ( line_ != null ) {
            parent_.getChildren().remove( line_ );// remove that line so that we can creat a new connection
        }
        Bounds parentBounds = parent_.getBoundsInParent();
        Bounds bounds = circle.localToScene( circle.getBoundsInLocal() );
        line_ = new Line();
        line_.setStrokeWidth( 4 );
        line_.setStartX( bounds.getCenterX() - parentBounds.getMinX() );
        line_.setStartY( bounds.getCenterY() - parentBounds.getMinY() );
        line_.setEndX( e.getSceneX() );
        line_.setEndY( e.getSceneY() );

        parent_.getChildren().add( line_ );
    }




    void startDrag(MouseEvent e) {
        // Cursor's Initial Position
        mouseStartDragX = e.getSceneX();
        mouseStartDragY = e.getSceneY();
        // Set widget xy position to mouse's xy position
        widgetStartDragX = this.getLayoutX();
        widgetStartDragY = this.getLayoutY();
    }

    void handleDrag(MouseEvent e, Circle output, Line line) {
        // New Position minus initial position
        double mouseDelX = e.getSceneX() - mouseStartDragX;
        double mouseDelY = e.getSceneY() - mouseStartDragY;
        // Difference of new Pos and initial Pos + Widget's initial Pos
        this.relocate( widgetStartDragX + mouseDelX , widgetStartDragY + mouseDelY );

        if (line_ != null) {
            Bounds anchorBound = parent_.getBoundsInParent();
            Bounds bounds = output.localToScene( output.getBoundsInLocal() );
            line_.setStartX(bounds.getCenterX() - anchorBound.getMinX());
            line_.setStartY(bounds.getCenterY() - anchorBound.getMinY());
        }

    }





    void handleSlider(MouseEvent e, Slider slider, Label title) {
        int value = (int) slider.getValue();
        title.setText( name_ + " (" + value + " Hz)" );
        switch (name_) {
            case "Sine Wave":
                ((SineWave) audioComponent_).setFreq( slider.getValue() );
                break;
            case "Square Wave":
                ((Square) audioComponent_).setFreq( slider.getValue() );
                break;
        }

    }

    void closeWidget() {
        parent_.getChildren().remove( this );
        SynthesizeApplication.connectedWg.remove(this);
        if( line_ != null ) {
            parent_.getChildren().remove( line_ );
        }
    }

    public AudioComponent getAudioComponent() {
        return audioComponent_;
    }
}
