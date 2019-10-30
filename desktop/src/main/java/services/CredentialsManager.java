package services;

import javax.crypto.SecretKey;
import java.util.Date;
import java.util.prefs.Preferences;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;
import io.jsonwebtoken.security.Keys;
import models.LoginRequest;
import org.apache.commons.lang3.time.DateUtils;

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

    public void saveFirstAuthToken(LoginRequest request) {
        Claims claims = Jwts.claims();
        claims.put("username", request.getUsager());
        claims.put("password", request.getMot_de_passe());

        Date expiryTime = DateUtils.addHours(new Date(), 1);
        SecretKey key = Keys.hmacShaKeyFor((HTTP_HEADER_AUTHORIZATION + "inf3995" + AUTH_TOKEN).getBytes());

        String token = Jwts.builder().setClaims(claims)
                .setExpiration(expiryTime)
                .signWith(key, SignatureAlgorithm.HS256)
                .compact();

        preferences.put("Authorization", token);
    }
}
