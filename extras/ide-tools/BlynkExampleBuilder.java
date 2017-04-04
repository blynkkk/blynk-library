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

public class BlynkExampleBuilder implements Tool {
  private Editor editor;

  public void init(Editor editor) {
    this.editor = editor;
  }

  public String getMenuTitle() {
    return "Blynk: Example Builder";
  }

  public void run() {
    Runnable runnable = () -> {
      try {
        Base.openURL(tr("http://examples.blynk.cc/"));
      } catch (Exception e) {
        editor.statusError("Blynk cannot open Example Builder");
        System.err.println(e);
        //e.printStackTrace(System.err);
        return;
      }
    };

    Thread thread = new Thread(runnable);
    thread.start();
  }
}
