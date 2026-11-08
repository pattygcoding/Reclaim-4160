// SVG -> PNG rasterizer for Reclaim-4160.
// All game art is authored as inline SVG here and rendered to PNG at build/setup
// time so it loads reliably under both native SDL2 and the Emscripten port.
//
//   npm install
//   npm run assets
//
import { Resvg } from '@resvg/resvg-js';
import { mkdirSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const here = dirname(fileURLToPath(import.meta.url));
const outDir = join(here, '..', 'assets', 'images');
mkdirSync(outDir, { recursive: true });

const items = [];
/** @param {string} name @param {number} width @param {string} svg */
const add = (name, width, svg) => items.push({ name, width, svg });

add('player', 96, `
<svg xmlns="http://www.w3.org/2000/svg" width="96" height="150" viewBox="0 0 96 150">
  <defs>
    <linearGradient id="shirt" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#2bb3a3"/><stop offset="1" stop-color="#1f8578"/></linearGradient>
    <linearGradient id="skin" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#ffd9b3"/><stop offset="1" stop-color="#f2c193"/></linearGradient>
  </defs>
  <rect x="34" y="104" width="14" height="38" rx="6" fill="#33405c"/>
  <rect x="50" y="104" width="14" height="38" rx="6" fill="#3a486a"/>
  <rect x="30" y="136" width="20" height="11" rx="5" fill="#22203a"/>
  <rect x="48" y="136" width="20" height="11" rx="5" fill="#22203a"/>
  <rect x="30" y="58" width="38" height="52" rx="14" fill="url(#shirt)"/>
  <rect x="60" y="66" width="14" height="34" rx="7" fill="#25a094"/>
  <circle cx="50" cy="40" r="24" fill="url(#skin)"/>
  <path d="M26 34 q24 -28 48 0 q-6 -20 -24 -20 q-18 0 -24 20 z" fill="#4b3a2f"/>
  <circle cx="42" cy="38" r="3.2" fill="#22203a"/>
  <circle cx="58" cy="38" r="3.2" fill="#22203a"/>
  <rect x="36" y="44" width="30" height="16" rx="7" fill="#dff1ff" stroke="#b9dcf5" stroke-width="2"/>
</svg>`);

add('shadow', 140, `
<svg xmlns="http://www.w3.org/2000/svg" width="140" height="44" viewBox="0 0 140 44">
  <defs><radialGradient id="s" cx="50%" cy="50%" r="50%">
    <stop offset="0" stop-color="rgba(0,0,0,0.42)"/>
    <stop offset="70%" stop-color="rgba(0,0,0,0.16)"/>
    <stop offset="100%" stop-color="rgba(0,0,0,0)"/>
  </radialGradient></defs>
  <ellipse cx="70" cy="22" rx="66" ry="18" fill="url(#s)"/>
</svg>`);

const germ = (id1, id2, spike, eyeY, angry) => `
<svg xmlns="http://www.w3.org/2000/svg" width="72" height="72" viewBox="0 0 72 72">
  <defs><radialGradient id="g${id1}" cx="40%" cy="32%" r="72%">
    <stop offset="0" stop-color="${id1}"/><stop offset="1" stop-color="${id2}"/>
  </radialGradient></defs>
  <g fill="${spike}">
    <circle cx="12" cy="30" r="6"/><circle cx="60" cy="30" r="6"/>
    <circle cx="20" cy="14" r="6"/><circle cx="52" cy="14" r="6"/>
    <circle cx="20" cy="52" r="6"/><circle cx="52" cy="52" r="6"/>
  </g>
  <circle cx="36" cy="36" r="26" fill="url(#g${id1})"/>
  <circle cx="27" cy="${eyeY}" r="7" fill="#fff"/>
  <circle cx="45" cy="${eyeY}" r="7" fill="#fff"/>
  <circle cx="28" cy="${eyeY + 1}" r="3" fill="#221e30"/>
  <circle cx="44" cy="${eyeY + 1}" r="3" fill="#221e30"/>
  ${angry
    ? '<path d="M21 24 l9 5 M51 24 l-9 5" stroke="#6b1d12" stroke-width="3" stroke-linecap="round"/>'
    : ''}
  <path d="M30 47 q6 5 12 0" stroke="#7a4a12" stroke-width="2.5" fill="none" stroke-linecap="round"/>
</svg>`;

add('enemy_patroller', 72, germ('#ffe08a', '#f0a92e', '#e59b1f', 34, false));
add('enemy_chaser', 72, germ('#ff9a7e', '#e2452f', '#c93622', 34, true));
add('enemy', 72, germ('#ffe08a', '#f0a92e', '#e59b1f', 34, false));

add('tp', 64, `
<svg xmlns="http://www.w3.org/2000/svg" width="64" height="64" viewBox="0 0 64 64">
  <defs><linearGradient id="paper" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#ffffff"/><stop offset="1" stop-color="#e6ebf2"/></linearGradient></defs>
  <rect x="8" y="20" width="48" height="34" rx="9" fill="url(#paper)" stroke="#d7dee8" stroke-width="2"/>
  <rect x="8" y="30" width="48" height="12" fill="#6bd6b0"/>
  <rect x="8" y="30" width="48" height="12" fill="none" stroke="#4fb99a" stroke-width="1.5"/>
  <circle cx="22" cy="26" r="4" fill="#cfd8e3"/>
  <circle cx="42" cy="26" r="4" fill="#cfd8e3"/>
  <path d="M20 12 l2 4 l4 2 l-4 2 l-2 4 l-2 -4 l-4 -2 l4 -2 z" fill="#ffc759"/>
</svg>`);

add('sanitizer', 48, `
<svg xmlns="http://www.w3.org/2000/svg" width="48" height="72" viewBox="0 0 48 72">
  <defs><linearGradient id="bot" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#5fd6a8"/><stop offset="1" stop-color="#2f8f6a"/></linearGradient></defs>
  <rect x="20" y="8" width="8" height="12" fill="#2f8f6a"/>
  <rect x="14" y="16" width="18" height="8" rx="3" fill="#37a079"/>
  <rect x="26" y="6" width="12" height="6" rx="3" fill="#37a079"/>
  <rect x="8" y="24" width="32" height="44" rx="9" fill="url(#bot)" stroke="#2a7f5f" stroke-width="2"/>
  <rect x="12" y="36" width="24" height="20" rx="4" fill="#ffffff" opacity="0.92"/>
  <rect x="16" y="42" width="16" height="3" rx="1.5" fill="#6bd6b0"/>
  <rect x="16" y="48" width="11" height="3" rx="1.5" fill="#6bd6b0"/>
</svg>`);

add('cart', 120, `
<svg xmlns="http://www.w3.org/2000/svg" width="120" height="84" viewBox="0 0 120 84">
  <path d="M6 10 h16 l9 40" stroke="#9aa4b2" stroke-width="6" fill="none" stroke-linecap="round"/>
  <path d="M26 24 h88 l-11 34 h-62 z" fill="#cfd8e3" stroke="#9aa4b2" stroke-width="3" stroke-linejoin="round"/>
  <g stroke="#9aa4b2" stroke-width="2" opacity="0.7">
    <path d="M42 24 l-7 34 M60 24 l-5 34 M78 24 l-3 34"/>
  </g>
  <circle cx="46" cy="72" r="8" fill="#5a6472"/><circle cx="46" cy="72" r="3" fill="#cfd8e3"/>
  <circle cx="90" cy="72" r="8" fill="#5a6472"/><circle cx="90" cy="72" r="3" fill="#cfd8e3"/>
</svg>`);

add('checkout', 200, `
<svg xmlns="http://www.w3.org/2000/svg" width="200" height="150" viewBox="0 0 200 150">
  <rect x="6" y="78" width="188" height="66" rx="8" fill="#26a69a"/>
  <rect x="6" y="78" width="188" height="12" rx="6" fill="#2fbfae"/>
  <rect x="18" y="64" width="120" height="18" rx="6" fill="#3a486a"/>
  <g stroke="#4c5c86" stroke-width="2"><path d="M30 64 v18 M46 64 v18 M62 64 v18 M78 64 v18 M94 64 v18 M110 64 v18 M126 64 v18"/></g>
  <rect x="140" y="34" width="48" height="44" rx="6" fill="#33405c"/>
  <rect x="146" y="40" width="36" height="20" rx="3" fill="#6bd6b0"/>
  <rect x="150" y="44" width="26" height="4" fill="#2fbfae"/>
  <rect x="150" y="50" width="18" height="4" fill="#2fbfae"/>
  <rect x="20" y="30" width="96" height="24" rx="6" fill="#ffc759"/>
  <g fill="#221e30"><rect x="30" y="40" width="56" height="4" rx="2"/></g>
  <circle cx="40" cy="42" r="9" fill="#ff6f61" stroke="#221e30" stroke-width="2"/>
</svg>`);

add('icon_heart', 40, `
<svg xmlns="http://www.w3.org/2000/svg" width="40" height="40" viewBox="0 0 40 40">
  <path d="M20 34 C -2 18 6 2 20 12 C 34 2 42 18 20 34 Z" fill="#ff5d5d" stroke="#d64545" stroke-width="2"/>
  <ellipse cx="14" cy="15" rx="4" ry="3" fill="#ffb3b3"/>
</svg>`);

// ---- Rasterize everything ----
for (const { name, width, svg } of items) {
  const resvg = new Resvg(svg, { fitTo: { mode: 'width', value: width }, background: 'rgba(0,0,0,0)' });
  const png = resvg.render().asPng();
  writeFileSync(join(outDir, `${name}.png`), png);
  console.log(`  ${name}.png  ${width}px  ${png.length} bytes`);
}
console.log(`\nGenerated ${items.length} textures -> assets/images/`);

