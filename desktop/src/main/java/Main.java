import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.util.Objects;

public class Main extends Application {
  @Override
  public void start(Stage primaryStage) throws Exception {
    Parent login = FXMLLoader.load(
            Objects.requireNonNull(getClass().getClassLoader().getResource("views/Login.fxml"))
    );
    primaryStage.setTitle("Admin manager");
    primaryStage.setScene(new Scene(login));
    primaryStage.show();
  }

  public static void main(String[] args) {
    launch(args);
  }
}
