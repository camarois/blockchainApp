import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.util.Objects;

public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        BorderPane borderPane = new BorderPane();
        Parent login = FXMLLoader.load(
                Objects.requireNonNull(getClass().getClassLoader().getResource("views/Login.fxml"))
        );
        borderPane.setCenter(login);
        primaryStage.setTitle("Admin manager");
        primaryStage.setScene(new Scene(borderPane, 600, 400));
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
