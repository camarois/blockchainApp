package models;

public class LoginRequest {
    String usager;
    //CHECKSTYLE.OFF: MemberName
    String mot_de_passe;
    //CHECKSTYLE.ON: MemberName

    public LoginRequest(String username, String password) {
        this.usager = username;
        this.mot_de_passe = password;
    }
}
