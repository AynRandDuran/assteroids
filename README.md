# Randy's Rowdy Rocket Roundup
Little asteroids-like I made using Raylib. Yes, the code is gross. I'm going to rewrite this in SDL once I make the time for it.

## Building
Install Raylib, maybe make sure it's on your path. Static linking got annoying so do it yourself.<br>
Just `make`, and target `win_game` if you're under Wangblows. You'll need mingw, which is distributed with Raylib (God knows why).<br>
I'll dig around for an older static windows build to share, I don't have a Windows machine now so I can't really actively test.<br>

## Playing
`JIL` controls your ship. Your arrow keys should work too. Technically you only need `UP`/`I` to start the round.<br>
`S` to shoot your main gun. Occasionally, the shotgun powerup will float by. Grab it to replace your main gun with shotgun blasts for the next few shots.<br>
`A` when your nose turns red after some amount of kills. Launch a devastating blackhole that expands as more asteroids fall into it.<br>
Rarely, a destroyed asteroid will drop a shield powerup that makes you invincible for the next several seconds.<br>
`H` for a help menu explaining things.<br>
`ESC` should quit.<br>

## Other
Thanks to Raysan for Raylib, [found here](https://www.raylib.com/)
