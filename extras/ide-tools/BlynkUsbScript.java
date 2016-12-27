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
import processing.app.helpers.OSUtils;
import processing.app.helpers.FileUtils;
import processing.app.helpers.PreferencesMap;
import processing.app.Base;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.DefaultCaret;
import java.nio.file.*;

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

  private void captureIO(final InputStream src, JTextArea textArea) {
    new Thread(() -> {
      Scanner sc = new Scanner(src);
      while (sc.hasNextLine()) {
        textArea.append(sc.nextLine() + "\n");
      }
      //} catch (IOException e) {
    }).start();
  }

  public void run() {
    Runnable runnable = () -> {
      try {
        //this.editor.serialMonitor.suspend();
        Font consoleFont = Theme.getFont("console.font");
        Font editorFont = PreferencesData.getFont("editor.font");
        Font font = Theme.scale(new Font(consoleFont.getName(), consoleFont.getStyle(), editorFont.getSize()));

        String warning = "Ensure that Serial Monitor and Plotter are closed when using this tool.\n" +
          "It uses same port and speed as Serial Monitor\n\n";

        String serial_port = PreferencesData.get("serial.port");
        String serial_debug_rate = PreferencesData.get("serial.debug_rate");
        String sketchbook_path = PreferencesData.get("sketchbook.path");

        Path blynk_path = Paths.get(sketchbook_path, "libraries", "Blynk");

        String full_cmd;
        if (OSUtils.isWindows()) {
          String script = "blynk-ser.bat";
          Path script_path = Paths.get(blynk_path.toString(), "scripts", script);

          String args = " -c " + serial_port + " -b " + serial_debug_rate;
          full_cmd = "echo." + warning.replace("\n"," & echo.") + " & \"" + script_path.toString() + "\"" + args;
          full_cmd = "cmd /C start \"Blynk\" cmd /C \"" + full_cmd + "\"";

          //System.err.println(full_cmd);

          final Process p = Runtime.getRuntime().exec(full_cmd);

          p.waitFor();
          p.destroy();
        } else {

          String script = "blynk-ser.sh";
          Path script_path = Paths.get(blynk_path.toString(), "scripts", script);

          String args = " -c " + serial_port + " -b " + serial_debug_rate;
          full_cmd = script_path.toString() + args;

          //System.err.println(full_cmd);

          JTextArea textArea = new JTextArea(warning);
          textArea.setRows(16);
          textArea.setColumns(120);
          textArea.setEditable(false);
          textArea.setLineWrap(true);
          textArea.setFont(font);

          DefaultCaret caret = (DefaultCaret)textArea.getCaret();
          caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);

          textArea.setCaretPosition(textArea.getDocument().getLength());

          JScrollPane scrollPane = new JScrollPane(textArea);
          //scrollPane.setPreferredSize( new Dimension(textArea.getPreferredSize().width, 500 ) );

          JFrame frame = new JFrame(script + args);
          frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
          frame.getContentPane().add(scrollPane, BorderLayout.CENTER);
          frame.pack();
          frame.setVisible(true);

          final Process p = Runtime.getRuntime().exec(full_cmd);

          captureIO(p.getInputStream(), textArea);
          captureIO(p.getErrorStream(), textArea);

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
        }

      } catch (Exception e) {
        editor.statusError("Blynk USB script failed");
        System.err.println(e);
        //e.printStackTrace(System.err);
        return;
      }
    };

    Thread thread = new Thread(runnable);
    thread.start();
  }
}
