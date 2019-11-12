package models;

import com.google.gson.annotations.SerializedName;

public class NewUserRequest {
  @SerializedName("usager")
  private String username;
  @SerializedName("mot_de_passe")
  private String password;
  @SerializedName("isEditor")
  private int isEditor;

  public NewUserRequest(String username, String password, int isEditor) {
    this.username = username;
    this.password = password;
    this.isEditor = isEditor;
  }

  public String getUsername() {
    return username;
  }
  public String getPassword() {
    return password;
  }
  public int getisEditor() {
    return isEditor;
  }
}
