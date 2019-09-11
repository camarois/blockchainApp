package controllers;

import javafx.event.ActionEvent;
import org.w3c.dom.Document;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.File;
import java.io.InputStream;
import java.net.*;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class Application {
    public void onClickHelloWorld(ActionEvent event) throws Exception {
        InputStream stream = getClass().getClassLoader().getResourceAsStream("values/strings.xml" );

        // Parse the XML file
        Document document = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
        String host = document.getElementsByTagName("host").item(0).getTextContent();

        // Request a response from the provided URL.
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(host + "/lol"))
                .build();
        client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
                .thenApply(HttpResponse::body)
                .thenAccept(System.out::println)
                .join();
    }
}

