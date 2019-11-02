package services;

import java.util.prefs.Preferences;

public class CredentialsManager {
    private static final String HTTP_HEADER_AUTHORIZATION = "Authorization";
    private static final String AUTH_TOKEN = "authorization_token";
    private static Preferences preferences;
    private static CredentialsManager instance;

    public static void init() {
        preferences = Preferences.userNodeForPackage(CredentialsManager.class);
    }

    public static CredentialsManager getInstance() {
        if (instance == null) {
            instance = new CredentialsManager();
            instance.init();
        }
        return instance;
    }

    public void saveAuthToken(String token) {
        preferences.put("Authorization", token);
    }

    public String getAuthToken() {
        return preferences.get("Authorization", "defaultToken");
    }
}
