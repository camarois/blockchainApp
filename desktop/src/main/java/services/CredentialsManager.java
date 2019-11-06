package services;

import java.util.prefs.Preferences;

class CredentialsManager {
    private static final String HTTP_HEADER_AUTHORIZATION = "Authorization";
    private static Preferences preferences;
    private static CredentialsManager instance;

    private static void init() {
        preferences = Preferences.userNodeForPackage(CredentialsManager.class);
    }

    static CredentialsManager getInstance() {
        if (instance == null) {
            instance = new CredentialsManager();
            init();
        }
        return instance;
    }

    static void saveAuthToken(String token) {
        preferences.put(HTTP_HEADER_AUTHORIZATION, token);
    }

    String getAuthToken() {
        return preferences.get(HTTP_HEADER_AUTHORIZATION, "defaultToken");
    }

}
