package models;

public class LoginRequest {
    String usager;
    //CHECKSTYLE.OFF: MemberName
    String mot_de_passe;
    //CHECKSTYLE.ON: MemberName

    public LoginRequest(String username, String password) {
        usager = username;
        mot_de_passe = password;
    }

    public String getUsager() {
        return usager;
    }

    public String getMot_de_passe() {
        return mot_de_passe;
    }
}
