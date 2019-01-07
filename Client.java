

import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    String host;
    int port;
    String file;

    public Client(String host, int port, String file) {
        this.host = host;
        this.port = port;
        this.file = file;
    }

    public Client() {
    }

    public void setHost(String host) {
        this.host = host;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public void setFile(String file) {
        this.file = file;
    }

    public static void main(String[] args) throws IOException {
        Client fc = new Client();
        Scanner scanner = new Scanner(System.in);

        System.out.println("Wpisz lokalizacje pliku  np. C:/Users/tom/Desktop/Beztytułu.png");
        fc.setFile(scanner.nextLine());
        System.out.println("Wpisz ip przez które chcesz się połączyć:");
        fc.setHost(scanner.nextLine());
        System.out.println("Wpisz port:");
        fc.setPort(scanner.nextInt());

        scanner.close();

        Socket s = new Socket(fc.host, fc.port);

        DataOutputStream dos = new DataOutputStream(s.getOutputStream());
        FileInputStream fis = new FileInputStream(fc.file);
        byte[] buffer = new byte[4096];

        while (fis.read(buffer) > 0) {
            dos.write(buffer);
        }

        fis.close();
        dos.close();

    }

}

