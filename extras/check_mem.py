Import("env")

import os
import re
import sys

from platformio.proc import exec_command


WARNING_THRESHOLD = 0.90  # 90%


def _configure_defaults():
    env.Replace(
        SIZECHECKCMD="$SIZETOOL -B -d $SOURCES",
        SIZEPROGREGEXP=r"^(\d+)\s+(\d+)\s+\d+\s",
        SIZEDATAREGEXP=r"^\d+\s+(\d+)\s+(\d+)\s+\d+",
    )


def _get_size_output(source):
    cmd = env.get("SIZECHECKCMD")
    if not cmd:
        return None

    if not isinstance(cmd, list):
        cmd = cmd.split()

    cmd = [arg.replace("$SOURCES", str(source[0])) for arg in cmd if arg]

    sysenv = os.environ.copy()
    sysenv["PATH"] = str(env["ENV"]["PATH"])

    result = exec_command(env.subst(cmd), env=sysenv)
    if result["returncode"] != 0:
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
        sys.stderr.write(
            "Warning: {} usage is {:.1f}% ({} / {} bytes), which exceeds {:.0f}%\n".format(
                label,
                ratio * 100.0,
                used,
                total,
                WARNING_THRESHOLD * 100.0,
            )
        )
    else:
        print(
            "{} usage is OK: {:.1f}% ({} / {} bytes)".format(
                label, ratio * 100.0, used, total
            )
        )


def check_memory_threshold(source, target, env_):
    del target, env_  # unused

    if not env.get("BOARD"):
        return

    if not env.get("SIZECHECKCMD") and not env.get("SIZETOOL"):
        return

    program_max_size = int(env.BoardConfig().get("upload.maximum_size", 0))
    data_max_size = int(env.BoardConfig().get("upload.maximum_ram_size", 0))

    if program_max_size == 0:
        return

    if not env.get("SIZECHECKCMD") and not env.get("SIZEPROGREGEXP"):
        _configure_defaults()

    output = _get_size_output(source)
    program_size = _calculate_size(output, env.get("SIZEPROGREGEXP"))
    data_size = _calculate_size(output, env.get("SIZEDATAREGEXP"))

    print('Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"')

    _warn_if_high("RAM", data_size, data_max_size)
    _warn_if_high("Flash", program_size, program_max_size)

    # Keep PlatformIO's original hard limits behavior
    if data_max_size and data_size > data_max_size:
        sys.stderr.write(
            "Warning! The data size (%d bytes) is greater than maximum allowed (%s bytes)\n"
            % (data_size, data_max_size)
        )

    if program_size > program_max_size:
        sys.stderr.write(
            "Error: The program size (%d bytes) is greater than maximum allowed (%s bytes)\n"
            % (program_size, program_max_size)
        )
        env.Exit(1)


env.AddPostAction("$PROGPATH", check_memory_threshold)