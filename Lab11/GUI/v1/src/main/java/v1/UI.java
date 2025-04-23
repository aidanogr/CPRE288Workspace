package v1;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.general.Dataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.Socket;
import java.util.Scanner;


///A WARNING FOR ALL DEBUGGING HEADACHES: I THINK ALL INSTANCES OF UI's MUST BE CLOSED BEFORE STARTING ANOTHER
@SuppressWarnings("serial")
public class UI extends JFrame {
    private XYSeries series;
    private Socket socket;
    private BufferedReader reader;
    private PrintWriter writer;
    private JTextField commandField;
    private JFreeChart chart;

    public UI() {
        setTitle("CyBot UI");
        setSize(800, 600);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        series = new XYSeries("Data");
        XYSeriesCollection dataset = new XYSeriesCollection(series);
        chart = ChartFactory.createXYLineChart(
                "IR Sensor", "angle", "raw", dataset);

        ChartPanel chartPanel = new ChartPanel(chart);
        add(chartPanel, BorderLayout.CENTER);

        commandField = new JTextField();
        commandField.addActionListener(e -> sendCommand());
        add(commandField, BorderLayout.SOUTH);

        setVisible(true);
        connectAndListen();
    }
    
    private void connectAndListen() {
        new Thread(() -> {
            try {
                System.out.println("Connecting to 192.168.1.1:288...");
                socket = new Socket("192.168.1.1", 288);
                reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                writer = new PrintWriter(socket.getOutputStream(), true);
                System.out.println("Connected.");

                String s = ""; Scanner scanner = new Scanner("");
                int split = 0;
                //ALL COMMANDS ARE FOLLOWED BY A NEW LINE NOW 
                while ((s = reader.readLine()) != null) {
                    System.out.println(s); // Just print each character as received
                    if(s.equals("start scan")) {
                    	while(!(s = reader.readLine()).equals("end scan")) {
                    		split = s.indexOf(',');
                    		this.series.add(Integer.valueOf(s.substring(0, split)), Integer.valueOf(s.substring(split + 1, s.length())));
                    	}
                    	this.chart.plotChanged(null); //idk if this is necessary 
                    	this.repaint();				  //this is definitely necessary
                    }
                    if(s.equals("forwards")) {
                    	split = s.indexOf(',');
                    	System.out.println(Integer.valueOf(s.substring(split+1, s.length())));
                    }
                }
                System.out.println("\nConnection closed.");
                socket.close();
                scanner.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

    //this doesnt do anything right now
    private void sendCommand() {
        if (writer != null) {
            String command = commandField.getText();
            writer.println(command);
            commandField.setText("");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(UI::new);
    }
}

