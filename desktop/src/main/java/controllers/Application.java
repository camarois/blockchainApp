package controllers;

import javafx.event.ActionEvent;
import org.w3c.dom.Document;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.net.*;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class Application {
    public void onClickHelloWorld(ActionEvent event) throws Exception {
        InputStream stream = getClass().getClassLoader().getResourceAsStream("values/strings.xml" );

        Document document = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
        String host = document.getElementsByTagName("host").item(0).getTextContent();

        try {
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(host + "/lol"))
                    .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            System.out.println(response.body());
        } catch (Exception e) {
            System.out.println("Exception occurred, http call didn't work");
        }
    }
}

