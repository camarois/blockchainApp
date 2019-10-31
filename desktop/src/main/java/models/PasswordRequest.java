package models;

public class PasswordRequest {
    private String ancien;
    private String nouveau;

    public PasswordRequest(String oldPassword, String newPassword) {
        ancien = oldPassword;
        nouveau = newPassword;
    }
}
