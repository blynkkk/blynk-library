/**
 * @file       BlynkUpdater.java
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Dec 2016
 * @brief
 *
 * It is not permitted to use this source code for other purposes,
 * except running scripts of original Blynk library.
 *
 */

package com.github.blynk.arduino.tools;

import java.io.*;
import java.util.*;
import java.lang.Runnable;
import java.lang.Thread;
import java.util.stream.Collectors;

import processing.app.Editor;
import processing.app.PreferencesData;
import processing.app.tools.Tool;

import processing.app.BaseNoGui;
import processing.app.I18n;
import processing.app.helpers.FileUtils;
import processing.app.helpers.PreferencesMap;
import processing.app.Base;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import processing.app.Theme;

import static processing.app.I18n.tr;
import processing.app.I18n;

public class BlynkUsbScript implements Tool {
  private Editor editor;

  public void init(Editor editor) {
    this.editor = editor;
  }

  public String getMenuTitle() {
    return "Blynk: Run USB script";
  }


  public void run() {
    Runnable runnable = () -> {
      try {
        //this.editor.serialMonitor.suspend();
        Font consoleFont = Theme.getFont("console.font");
        Font editorFont = PreferencesData.getFont("editor.font");
        Font font = Theme.scale(new Font(consoleFont.getName(), consoleFont.getStyle(), editorFont.getSize()));

        JTextArea textArea = new JTextArea(
          "Ensure that Serial Monitor and Plotter are closed when using this tool.\n" +
          "It uses same port and speed as Serial Monitor\n\n"
        );
        textArea.setRows(16);
        textArea.setColumns(120);
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setFont(font);

        JScrollPane scrollPane = new JScrollPane(textArea);
        //scrollPane.setPreferredSize( new Dimension(textArea.getPreferredSize().width, 500 ) );

        String serial_port = PreferencesData.get("serial.port");
        String serial_serial_rate = PreferencesData.get("serial.debug_rate");
        String sketchbook_path = PreferencesData.get("sketchbook.path");

        String blynk_path = sketchbook_path + "/libraries/Blynk/";
        String scripts_path = blynk_path + "/scripts/";
        String cmd = "blynk-ser.sh --comm=" + serial_port + " --baud=" + serial_serial_rate;

        JFrame frame = new JFrame(cmd);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);
        frame.pack();
        frame.setVisible(true);

        final Process p = Runtime.getRuntime().exec(scripts_path + cmd);

        new Thread(() -> {
          BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));

          try {
            String line = null; 
            while ((line = input.readLine()) != null) {
              textArea.append(line + "\n");
            }
          } catch (IOException e) {
          }
        }).start();

        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent windowEvent) {
              p.destroy();
            }
        });

        p.waitFor();
        p.destroy();
        frame.setVisible(false);
        frame.dispose();

      } catch (Exception e) {
        editor.statusError("Blynk USB script failed");
        System.err.println(e);
        return;
      }
    };

    Thread thread = new Thread(runnable);
    thread.start();
  }
}
