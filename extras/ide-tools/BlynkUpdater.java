/**
 * @file       BlynkUpdater.java
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Dec 2016
 * @brief
 *
 * It is not permitted to use this source code for other purposes,
 * except original Blynk library installation.
 *
 */

package com.github.blynk.arduino.tools;

import java.io.*;
import java.util.*;
import java.net.URL;
import java.lang.Runnable;
import java.lang.Thread;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipEntry;
import java.util.stream.Collectors;

import javax.swing.JOptionPane;

import processing.app.Editor;
import processing.app.PreferencesData;
import processing.app.tools.Tool;

import processing.app.BaseNoGui;
import processing.app.I18n;
import processing.app.helpers.FileUtils;
import processing.app.helpers.PreferencesMap;
import processing.app.Base;
import processing.app.tools.ZipDeflater;

import cc.arduino.contributions.libraries.ContributedLibrary;
import cc.arduino.contributions.filters.InstalledPredicate;
import cc.arduino.contributions.VersionComparator;

import static processing.app.I18n.tr;

public class BlynkUpdater implements Tool {
  private Editor editor;

  public void init(Editor editor) {
    this.editor = editor;
  }

  public String getMenuTitle() {
    return "Blynk: Check for updates";
  }

  private static ByteArrayOutputStream downloadFile(String url) throws IOException {
    System.out.println("Downloading '" + url + "'");
    System.out.print("...");

    InputStream is = new URL(url).openStream();
    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

    byte[] chunk = new byte[32 * 1024];
    int bytesRead;

    while ((bytesRead = is.read(chunk)) > 0) {
      System.out.print(".");
      outputStream.write(chunk, 0, bytesRead);
    }
    System.out.println(" done.");
    return outputStream;
  }

  private static boolean isSymlink(File file) throws IOException {
    if (file == null)
      throw new NullPointerException("File must not be null");
    File canon;
    if (file.getParent() == null) {
      canon = file;
    } else {
      File canonDir = file.getParentFile().getCanonicalFile();
      canon = new File(canonDir, file.getName());
    }
    return !canon.getCanonicalFile().equals(canon.getAbsoluteFile());
  }

  private static void updateFolder(File[] components, File tgtFolder) {
    for (File f: components) {
      try {
        File tgt = new File(tgtFolder, f.getName());
        if (isSymlink(tgt)) {
          System.out.println("Skipping " + f.getName() + " [symlink]");
        } else {
          System.out.println("Updating " + f.getName());
          FileUtils.recursiveDelete(tgt);
          tgt.mkdir();
          FileUtils.copy(f, tgt);
        }
      } catch (Exception e) {
        System.err.println(e);
      } finally {}
    }
  }

  public void run() {
    String lib_url = "https://raw.githubusercontent.com/blynkkk/blynk-library/master/library.properties";

    Runnable runnable = () -> {
      try {
        BaseNoGui.librariesIndexer.rescanLibraries();

        ByteArrayOutputStream last_lib_os = downloadFile(lib_url);
        ByteArrayInputStream  last_lib_is = new ByteArrayInputStream(last_lib_os.toByteArray()); 
        PreferencesMap properties = new PreferencesMap();
        properties.load(last_lib_is);

        String last_version = properties.get("version");
        System.out.println("Last version: " + last_version);

        File sketchbook_path = new File(PreferencesData.get("sketchbook.path"));
        File ide_path = new File(PreferencesData.get("runtime.ide.path"));

        List<ContributedLibrary> blynk_libs = BaseNoGui.librariesIndexer.getIndex().find("Blynk").stream().filter(new InstalledPredicate()).collect(Collectors.toList());
        if (blynk_libs.size() >= 2) {
          Base.showMessage(tr("Error"), tr("Error!"));
          return;
        }

        boolean needUpdate = false;
        if (blynk_libs.size() == 0) {
          needUpdate = true;
        } else {
          ContributedLibrary installed = blynk_libs.get(0);

          System.out.println("Installed version: " + installed.getVersion());
          final VersionComparator versionComparator = new VersionComparator();
          needUpdate = versionComparator.greaterThan(last_version, installed.getParsedVersion());
        }

        if (!needUpdate) {
          Base.showMessage(tr("No update needed"), tr("Blynk is up to date!"));
          return;
        }

        { // Ask for update
          Object[] options = { tr("Yes"), tr("No") };
          int result = JOptionPane.showOptionDialog(editor,
                                                    "Blynk v" + last_version + " is available.\n" +
                                                    "Do you want to update?",
                                                    tr("Update"),
                                                    JOptionPane.YES_NO_OPTION,
                                                    JOptionPane.QUESTION_MESSAGE,
                                                    null,
                                                    options,
                                                    options[0]);
          if (result != JOptionPane.YES_OPTION) {
            return;
          }
        }

        String zip_fn = "Blynk_Release_v" + last_version;
        String zip_url = "https://github.com/blynkkk/blynk-library/releases/download/v" + last_version + "/" + zip_fn + ".zip";

        File tmpFolder = null;
        ByteArrayOutputStream zip_os = downloadFile(zip_url);
        try {
          tmpFolder = FileUtils.createTempFolder();

          File tmpFile = new File(tmpFolder, zip_fn + ".zip");
          try(OutputStream outputStream = new FileOutputStream(tmpFile)) {
              zip_os.writeTo(outputStream);
          }
          zip_os = null;
          System.out.println("Unpacking to " + tmpFolder);
          ZipDeflater zipDeflater = new ZipDeflater(tmpFile, tmpFolder);
          zipDeflater.deflate();

          File tmpUnpackedFolder = new File(tmpFolder, zip_fn);

          // Update libs
          File tgtLibsFolder = new File(sketchbook_path, "libraries");
          File[] libraries = tmpUnpackedFolder.listFiles((dir, name) -> !(name.equals("tools") || name.equals("hardware")));
          updateFolder(libraries, tgtLibsFolder);

          // Update tools
          File tgtToolsFolder = new File(sketchbook_path, "tools");
          File tmpToolsFolder = new File(tmpUnpackedFolder, "tools");
          if (tmpToolsFolder.exists()) {
            updateFolder(tmpToolsFolder.listFiles(), tgtToolsFolder);
          }
        } catch (IOException e) {
        } finally {
          // delete zip created temp folder, if exists
          if (tmpFolder != null) {
            FileUtils.recursiveDelete(tmpFolder);
          }
        }

        BaseNoGui.librariesIndexer.rescanLibraries();

        { // Ask for a star
          Object[] options = { tr("Yes"), tr("Not now") };
          int result = JOptionPane.showOptionDialog(editor,
                                                    "Blynk libraries are succesfully updated!\n\n" +
                                                    "Would you like to give us a star on github?",
                                                    tr("Update"),
                                                    JOptionPane.YES_NO_OPTION,
                                                    JOptionPane.QUESTION_MESSAGE,
                                                    null,
                                                    options,
                                                    options[0]);
          if (result == JOptionPane.YES_OPTION) {
            Base.openURL(tr("https://github.com/blynkkk/blynk-library/releases/latest"));
          }
        }

        editor.statusNotice("Blynk libraries updated.");
/*
        for (ContributedLibrary l : libraries) {
          System.out.println(l.getVersion());
          //FileUtils.isSubDirectory(sketchbookLibrariesFolder, folder);
          if (FileUtils.isSubDirectory(sketchbook_path, l.getInstalledFolder())) {
            System.out.println("In folder: " + l.getInstalledFolder());
          }
        }
*/

      } catch (Exception e) {
        editor.statusError("Blynk update failed");
        System.err.println(e);
        return;
      }
    };

    Thread thread = new Thread(runnable);
    thread.start();
  }
}
