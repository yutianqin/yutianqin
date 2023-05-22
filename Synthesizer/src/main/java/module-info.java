module com.example.synthesizer {
    requires javafx.controls;
    requires javafx.fxml;
    requires org.junit.jupiter.api;
    requires java.desktop;


    opens com.example.synthesizer to javafx.fxml;
    exports com.example.synthesizer;
    exports com.example.synthesizer.Widgets;
    opens com.example.synthesizer.Widgets to javafx.fxml;
}