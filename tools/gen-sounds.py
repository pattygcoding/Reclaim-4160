"""
Procedural audio generator for Reclaim-4160.

Every sound effect and music loop is synthesized from scratch (sine/square/
triangle oscillators + ADSR envelopes + noise) and written as 16-bit mono WAV.
No third-party samples, so there are no licensing concerns.

    python tools/gen-sounds.py
"""
import math
import os
import random
import struct
import wave

SR = 22050
OUT = os.path.join(os.path.dirname(__file__), "..", "assets", "sounds")
os.makedirs(OUT, exist_ok=True)

random.seed(4160)


def write_wav(name: str, samples, sr: int = SR) -> None:
    path = os.path.join(OUT, name)
    peak = max(1e-6, max(abs(s) for s in samples)) if samples else 1.0
    scale = 0.92 / peak if peak > 0.92 else 1.0
    with wave.open(path, "w") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(sr)
        frames = bytearray()
        for s in samples:
            v = max(-1.0, min(1.0, s * scale))
            frames += struct.pack("<h", int(v * 32767))
        w.writeframes(bytes(frames))
    print(f"  {name:<20} {len(samples)/sr:5.2f}s  {os.path.getsize(path)} bytes")


def osc(shape: str, phase: float) -> float:
    if shape == "sine":
        return math.sin(2 * math.pi * phase)
    if shape == "square":
        return 1.0 if math.sin(2 * math.pi * phase) >= 0 else -1.0
    if shape == "triangle":
        p = phase % 1.0
        return 4 * abs(p - 0.5) - 1.0
    if shape == "saw":
        return 2 * (phase % 1.0) - 1.0
    return math.sin(2 * math.pi * phase)


def note(freq, dur, shape="square", vol=0.5, attack=0.01, release=0.05,
         glide_to=None, vibrato=0.0, sr=SR):
    n = int(dur * sr)
    out = [0.0] * n
    phase = 0.0
    for i in range(n):
        t = i / sr
        if t < attack:
            env = t / attack
        elif t > dur - release:
            env = max(0.0, (dur - t) / release)
        else:
            env = 1.0
        f = freq
        if glide_to is not None:
            f = freq + (glide_to - freq) * (t / dur)
        if vibrato:
            f *= 1.0 + vibrato * math.sin(2 * math.pi * 5.5 * t)
        phase += f / sr
        out[i] = osc(shape, phase) * vol * env
    return out


def noise(dur, vol=0.5, decay=True, sr=SR):
    n = int(dur * sr)
    return [random.uniform(-1, 1) * vol * ((1 - i / n) if decay else 1.0) for i in range(n)]


def mix(*parts):
    length = max(len(p) for p in parts) if parts else 0
    out = [0.0] * length
    for p in parts:
        for i, v in enumerate(p):
            out[i] += v
    return out


def concat(*parts):
    out = []
    for p in parts:
        out.extend(p)
    return out


def silence(dur, sr=SR):
    return [0.0] * int(dur * sr)


def arp(freqs, dur, shape="square", vol=0.3):
    return concat(*[note(f, dur, shape, vol) for f in freqs])


# ---------------------------------------------------------------- sound effects
write_wav("select.wav", note(880, 0.06, "square", 0.35, attack=0.002, release=0.03))
write_wav("confirm.wav", concat(note(660, 0.07, "square", 0.35), note(990, 0.10, "square", 0.35)))
write_wav("jump.wav", note(320, 0.16, "square", 0.32, glide_to=640, release=0.06))
write_wav("hit.wav", mix(note(220, 0.28, "square", 0.35, glide_to=80, release=0.1),
                         noise(0.22, 0.25)))
write_wav("stomp.wav", mix(note(150, 0.18, "square", 0.35, glide_to=70, release=0.08),
                           noise(0.14, 0.3)))
write_wav("pickup_tp.wav", arp([523.25, 659.25, 783.99], 0.06, "square", 0.3))
write_wav("pickup_health.wav", arp([440.0, 587.33, 659.25], 0.07, "sine", 0.35))
write_wav("complete.wav", arp([659.25, 783.99, 987.77, 1174.66], 0.08, "triangle", 0.32))
write_wav("win.wav", concat(note(523.25, 0.14, "square", 0.3), note(659.25, 0.14, "square", 0.3),
                            note(783.99, 0.14, "square", 0.3), note(1046.5, 0.30, "square", 0.32)))
write_wav("lose.wav", concat(note(392.0, 0.22, "square", 0.3), note(329.63, 0.22, "square", 0.3),
                             note(261.63, 0.40, "square", 0.32)))

# ---------------------------------------------------------------- music tracks


def add_at(buf, t, samples, sr=SR):
    start = int(t * sr)
    for i, v in enumerate(samples):
        idx = start + i
        if idx < len(buf):
            buf[idx] += v


def music_game():
    bpm = 132
    beat = 60.0 / bpm
    eighth = beat / 2
    scale = [523.25, 587.33, 659.25, 783.99, 880.0]  # C D E G A
    bass = [130.81, 164.81, 196.0, 164.81]
    beats = 16
    buf = [0.0] * int(beats * beat * SR)

    melody = [0, 2, 1, 4, 2, 3, 1, 0, 2, 4, 3, 1, 0, 2, 1, 3,
              0, 2, 1, 4, 2, 3, 4, 2, 1, 3, 2, 0, 4, 2, 1, 0]
    t = 0.0
    for m in melody:
        add_at(buf, t, note(scale[m], eighth * 0.92, "square", 0.13, attack=0.004, release=0.04))
        t += eighth

    t = 0.0
    for b in range(beats):
        add_at(buf, t, note(bass[b % len(bass)], beat * 0.9, "triangle", 0.22, attack=0.008, release=0.08))
        add_at(buf, t + beat * 0.5, noise(0.03, 0.05))
        t += beat
    return buf


def music_menu():
    bpm = 96
    beat = 60.0 / bpm
    scale = [392.0, 440.0, 523.25, 587.33, 659.25]  # G A C D E
    beats = 12
    buf = [0.0] * int(beats * beat * SR)
    melody = [0, 2, 3, 2, 1, 3, 4, 3, 2, 4, 3, 2]
    t = 0.0
    for m in melody:
        add_at(buf, t, note(scale[m], beat * 0.85, "sine", 0.22, attack=0.02, release=0.15, vibrato=0.004))
        add_at(buf, t, note(scale[m] / 2, beat * 0.85, "triangle", 0.12, attack=0.02, release=0.15))
        t += beat
    return buf


def music_win():
    bpm = 120
    beat = 60.0 / bpm
    notes = [523.25, 659.25, 783.99, 1046.5, 783.99, 1046.5]
    buf = [0.0] * int(len(notes) * beat * SR)
    t = 0.0
    for f in notes:
        add_at(buf, t, note(f, beat * 0.9, "square", 0.22, attack=0.005, release=0.1))
        t += beat
    return buf


def music_lose():
    bpm = 84
    beat = 60.0 / bpm
    notes = [392.0, 349.23, 311.13, 261.63]
    buf = [0.0] * int(len(notes) * beat * SR)
    t = 0.0
    for f in notes:
        add_at(buf, t, note(f, beat * 0.95, "triangle", 0.24, attack=0.02, release=0.2))
        add_at(buf, t, note(f / 2, beat * 0.95, "sine", 0.12, attack=0.02, release=0.2))
        t += beat
    return buf


write_wav("music_game.wav", music_game())
write_wav("music_menu.wav", music_menu())
write_wav("music_win.wav", music_win())
write_wav("music_lose.wav", music_lose())

