Import("env")

import os
import re
import sys

from platformio.proc import exec_command

WARNING_THRESHOLD = 0.9


def _configure_defaults():
    env.Replace(
        SIZECHECKCMD="$SIZETOOL -B -d $SOURCES",
        SIZEPROGREGEXP=r"^(\d+)\s+(\d+)\s+\d+\s",
        SIZEDATAREGEXP=r"^\d+\s+(\d+)\s+(\d+)\s+\d+",
    )


def _get_size_output(target):
    cmd = env.get("SIZECHECKCMD")
    if not cmd:
        return None

    if not isinstance(cmd, list):
        cmd = cmd.split()

    # In AddPostAction("$PROGPATH", ...), the built program is target[0]
    program_path = str(target[0])

    cmd = [arg.replace("$SOURCES", program_path) for arg in cmd if arg]

    sysenv = os.environ.copy()
    sysenv["PATH"] = str(env["ENV"]["PATH"])

    result = exec_command(env.subst(cmd), env=sysenv)
    if result["returncode"] != 0:
        print("SIZECHECKCMD failed:", " ".join(env.subst(cmd)))
        print("stdout:", result.get("out", ""))
        print("stderr:", result.get("err", ""))
        return None

    return result["out"].strip()


def _calculate_size(output, pattern):
    if not output or not pattern:
        return -1

    size = 0
    regexp = re.compile(pattern)

    for line in output.splitlines():
        line = line.strip()
        if not line:
            continue

        match = regexp.search(line)
        if match:
            size += sum(int(value) for value in match.groups())

    return size

def _warn_if_high(label, used, total):
    if total <= 0 or used < 0:
        return

    ratio = float(used) / float(total)
    if ratio >= WARNING_THRESHOLD:
        msg = "{} usage is {:.1f}% ({} / {} bytes), which exceeds {:.0f}%\n".format(
            label,
            ratio * 100.0,
            used,
            total,
            WARNING_THRESHOLD * 100.0,
        )
        if src := os.getenv("PLATFORMIO_CI_SRC"):
            print(f'::warning f"title=Size Warning:{src}::{msg}')
        else:
            print("WARNING:", msg)

    else:
        print(
            "{} usage is OK: {:.1f}% ({} / {} bytes)".format(
                label, ratio * 100.0, used, total
            )
        )


def check_memory_threshold(source, target, env):
    if not env.get("BOARD"):
        return

    if not env.get("SIZECHECKCMD") and not env.get("SIZETOOL"):
        return
    platform = env.PioPlatform()
    if "espressif32" in platform.name:
        program_max_size = 1310720
    elif "espressif8266" in platform.name:
        program_max_size = 1044464
    else:
        program_max_size = int(env.BoardConfig().get("upload.maximum_size", 0))
    data_max_size = int(env.BoardConfig().get("upload.maximum_ram_size", 0))

    if program_max_size == 0:
        return

    if not env.get("SIZECHECKCMD") and not env.get("SIZEPROGREGEXP"):
        _configure_defaults()

    output = _get_size_output(target)
    program_size = _calculate_size(output, env.get("SIZEPROGREGEXP"))
    data_size = _calculate_size(output, env.get("SIZEDATAREGEXP"))
    
    if data_size <= 0 or program_size <= 0:
        sys.stderr.write("Error: Unable to determine program or data size from output\n")
        env.Exit(1)

    _warn_if_high("RAM", data_size, data_max_size)
    _warn_if_high("Flash", program_size, program_max_size)


env.AddPostAction("$PROGPATH", check_memory_threshold)
