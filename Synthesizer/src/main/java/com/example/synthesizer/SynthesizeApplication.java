package com.example.synthesizer;

import com.example.synthesizer.Widgets.AudioComponentWidget;
import com.example.synthesizer.Widgets.VFSSineWaveWidget;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import javax.sound.sampled.*;
import java.io.IOException;
import java.util.ArrayList;


public class SynthesizeApplication extends Application {

    private AnchorPane mainCanvas_;
    public static Circle speaker_;
    public static ArrayList<AudioComponentWidget> connectedWg = new ArrayList<>();  //Store all the widgets
    public static ArrayList<AudioComponentWidget> allWidgets = new ArrayList<>();

    public Slider volsli ;
//    public Filter filter = new Filter( 0.5 );

    public Slider freqSlider;
    @Override
    public void start(Stage stage) throws IOException {

        /////
        // Parent( root )
        BorderPane root = new BorderPane();
        Scene scene = new Scene( root, 800, 600 );
        stage.setTitle( "Synthesizer" );


        /////
        // Top
        HBox nameSpace = new HBox(10);
        nameSpace.setPrefSize(800,20);
        nameSpace.setAlignment(Pos.CENTER);
        nameSpace.setStyle("-fx-background-color: white");

        Label name = new Label("ytq");
        nameSpace.getChildren().add( name );


        /////
        // Bottom
        // Horizontal Box: top play and stop button
        HBox playStopWidget = new HBox(10);
        playStopWidget.setPrefSize(800,40);
        playStopWidget.setAlignment(Pos.CENTER);
        playStopWidget.setStyle("-fx-background-color: white");
        // Play & Stop Buttons
        Button play_b = new Button("Play");
        play_b.setFont(Font.font(15));
        play_b.setOnAction( e -> play() );
        Button stop_b = new Button("Stop");
        stop_b.setFont(Font.font(15));
        stop_b.setOnAction( e -> stopplay() );
        playStopWidget.getChildren().addAll(play_b,stop_b);



        /////
        // Right
        VBox rightPanel = new VBox(); // Vertical Box
        rightPanel.setPadding(new Insets(10));
        rightPanel.setSpacing( 20 );
        rightPanel.setAlignment( Pos.CENTER_LEFT);
        rightPanel.setStyle("-fx-background-color: darkgrey");

        // Four Buttons on the right panel
        Button sineWaveBtn = new Button("Sine Wave");
        sineWaveBtn.setFont(Font.font(15));
        sineWaveBtn.setOnAction( e -> createComponent("Sine Wave"));
        rightPanel.getChildren().add( sineWaveBtn );

        Button square = new Button("Square Wave");
        square.setFont(Font.font(15));
        square.setOnAction( e -> createComponent("Square Wave"));
        rightPanel.getChildren().add( square );

        Button whitenoise = new Button("White Noise Wave");
        whitenoise.setFont(Font.font(15));
        whitenoise.setOnAction( e -> createComponent("White Noise Wave"));
        rightPanel.getChildren().add( whitenoise );

        Button vfssinewave = new Button("VFS Sine Wave");
        vfssinewave.setFont(Font.font(15));
        vfssinewave.setOnAction( e -> createComponent("VFS Sine Wave"));
        rightPanel.getChildren().add( vfssinewave );

        // Volume
        VBox vol = new VBox();
        Label volname = new Label();
        volname.setText("Volume");
        vol.getChildren().add( volname );
        volsli = new Slider();
        volsli.setOnMouseDragged( e -> handleVolsli( e , volsli , volname ) );
        vol.getChildren().add( volsli );
        vol.setPrefSize(100,50);
        vol.setAlignment(Pos.CENTER);
        vol.setStyle("-fx-background-color: white");
        rightPanel.getChildren().add( vol );


        /////
        // Center ( main )
        mainCanvas_ = new AnchorPane();
        mainCanvas_.setStyle( "-fx-background-color: lightblue" );
        speaker_ = new Circle( 600, 500, 30 );// Speaker (circle)
        speaker_.setFill(Color.BLACK);
        mainCanvas_.getChildren().add( speaker_ );



        /////
        // Arrange the layout of element on the root(AnchorPanel)
        root.setTop( nameSpace );
        root.setBottom( playStopWidget );
        root.setRight( rightPanel );
        root.setCenter( mainCanvas_ );



        /////
        // Last thing to do. Write once, don't worry about again...
        stage.setScene(scene);
        stage.show();
    }



    private void handleVolsli(MouseEvent e, Slider volsli, Label volname) {
        int value = (int) volsli.getValue();
        volname.setText( "Volume " + value );
    }

    private void stopplay() {
    }

    private void createComponent( String name ) {
        System.out.println("In create component");

        AudioComponent sinewave = new SineWave( 440 );
        AudioComponent square = new Square( 440 );
        AudioComponent whitenoise = new WhiteNoise();

        int count = 0;

        if ( connectedWg.isEmpty() ) {
            switch (name) {
                case "Sine Wave":
                    count++;
                    AudioComponentWidget sinew = new AudioComponentWidget( sinewave, mainCanvas_, name );
                    connectedWg.add( sinew );
                    break;
                case "Square Wave":
                    count++;
                    AudioComponentWidget squarew = new AudioComponentWidget( square, mainCanvas_, name );
                    connectedWg.add( squarew );
                    break;
                case "White Noise Wave":
                    count++;
                    AudioComponentWidget whitenoisew = new AudioComponentWidget( whitenoise, mainCanvas_, name );
                    connectedWg.add( whitenoisew );
                    break;
                case "VFS Sine Wave":
                    count++;
                    AudioComponentWidget vfssinew = new VFSSineWaveWidget( mainCanvas_, name );
                    connectedWg.add( vfssinew );
                    break;
                default:
                    System.out.println( "Nothing Generated" );
            }
        } else {
            switch (name) {
                case "Sine Wave":
                    AudioComponentWidget sinew = new AudioComponentWidget( sinewave, mainCanvas_, name );
                    count++;
                    connectedWg.add( sinew );
                    break;
                case "Square Wave":
                    AudioComponentWidget squarew = new AudioComponentWidget( square, mainCanvas_, name );
                    connectedWg.add( squarew );
                    count++;
                    break;
                case "White Noise Wave":
                    AudioComponentWidget whitenoisew = new AudioComponentWidget( whitenoise, mainCanvas_, name );
                    count++;
                    connectedWg.add( whitenoisew );
                    break;
                case "VFS Sine Wave":
                    AudioComponentWidget vfssinew = new VFSSineWaveWidget( mainCanvas_, name ); // Has its own input:
                    connectedWg.add( vfssinew );
                    break;
                default:
                    System.out.println( "Nothing Generated" );
            }
        }

    }



    private void play() {
        System.out.println( "Play pressed" );
        if ( connectedWg.size() == 0 ) {
            System.out.println( "Nothing connected" );
            return;
        }
        try {
            Clip c = AudioSystem.getClip();
            c.addLineListener( e -> handleAudioDone( e, c ) );
            Mixer mixer = new Mixer();
            for ( AudioComponentWidget acw : connectedWg ) {
                AudioComponent ac = acw.getAudioComponent();
                mixer.connectInput( ac );
            }
            Filter filter = new Filter( volsli.getValue() );
            filter.connectInput(mixer.getClip());
            byte[] data = filter.getClip().getData();
            AudioFormat format = new AudioFormat( 44100,16,1, true, false);
            c.open(format, data, 0, data.length);
            c.start();
        }
        catch ( LineUnavailableException e ) {}
    }



    private void handleAudioDone(LineEvent e, Clip c) {
        if ( e.getType() == LineEvent.Type.STOP ) {
            System.out.println("Close clip");
            c.close();
        }
    }


    public static void main(String[] args) {
        launch();
    }
}