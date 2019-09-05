package controllers;

import javafx.event.ActionEvent;
import java.net.*;
import java.io.*;

public class Application {
    public void onClickHelloWorld(ActionEvent event) throws Exception {

        URL url = new URL("http://localhost:10000/lol");
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");
        conn.setRequestProperty("Accept", "application/atom+xml");
        int status = conn.getResponseCode();

        BufferedReader in = new BufferedReader(
                new InputStreamReader(conn.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(status);
        System.out.println(content);

        conn.disconnect();
    }
}

