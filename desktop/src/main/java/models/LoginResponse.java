package models;

public class LoginResponse {
    boolean edition;

    public LoginResponse(boolean admin) {
        edition = admin;
    }

    public Boolean getEdition() {
        return edition;
    }
}
