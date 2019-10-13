package services;

import com.google.gson.Gson;
import constants.ServerUrls;

import java.io.InputStreamReader;

public class RestService {
    public static ServerUrls urls;

    static {
        InputStreamReader reader = new InputStreamReader(
                RestService.class.getClassLoader().getResourceAsStream("values/strings.json")
        );
        urls = new Gson().fromJson(reader, ServerUrls.class);
    }
}
