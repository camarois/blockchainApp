package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.control.Button;

import java.util.Objects;


public class Login {
  /**
   * Login btn event handler.
   * @param event event
   * @throws Exception Exception
   */
  public void onClickLogin(ActionEvent event) throws Exception {
    Button btn = (Button) event.getSource();
    Parent root = FXMLLoader.load(
        Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
    );
    btn.getScene().setRoot(root);
  }
}
