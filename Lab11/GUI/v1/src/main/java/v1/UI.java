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
///
///usage:
///run lab 11 as is 
///run ui with bot connected to wifi
///send commands in textbox of ui (hit enter) in the form "opcode, param1 (as integer), param2 (as integer)"
///example: 
///w,100,0  //move forwards 100
///p,0,180  //scan range (uses graph)
///
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
        commandField.addActionListener(e -> {
			try {
				sendCommand();
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		});
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

                String s = ""; 
                int split = 0;
                //ALL COMMANDS ARE FOLLOWED BY A NEW LINE NOW 
                while ((s = reader.readLine()) != null) {
                    System.out.println(s); // Just print each character as received
                    if(s.equals("start scan")) {
                    	while(!(s = reader.readLine()).equals("end scan")) {
                    		split = s.indexOf(',');
                    		this.series.add(Integer.valueOf(s.substring(0, split)), Integer.valueOf(s.substring(split + 1, s.length())));
                    	}
                    	this.repaint();				
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

    //this sends command in the form [char opcode][char param1][char param2]. Inputs are integers (except opcode
	// and are converted to chars before sent (i.e w,100,0)
    private void sendCommand() throws InterruptedException {
        if (writer != null) {
            String command = commandField.getText();
            int splits[] = new int[2];
            splits[0] = command.indexOf(',');
            splits[1] = command.substring(splits[0]+1).indexOf(',') + splits[0]+1;
            
            char opcode = command.charAt(splits[0]-1);

            int p = Integer.valueOf(command.substring(splits[0]+1, splits[1]));
            char param1 = (char) p;
            
            int p2 = Integer.valueOf(command.substring(splits[1]+1), command.length());
            char param2 = (char) p2;
            
            writer.print(opcode);
            Thread.sleep(20);
            writer.print(param1);
			Thread.sleep(20);
            writer.print(param2);
            Thread.sleep(20);
            writer.flush();
            
            System.out.println(opcode + " " + param1 + " " + param2);
            commandField.setText("");
        }
        else {
        	System.out.println("gate 2");
        }
    }

    ///Entry
    public static void main(String[] args) {
        SwingUtilities.invokeLater(UI::new);
    }
}

