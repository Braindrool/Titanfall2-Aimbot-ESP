Work in progress

https://i.imgur.com/t9QEuPN.png

Features include aimbot, esp w/ skeletons, smooth aim, configuration menu, FOV reticle, screenshot cleaner, etc...basically everything you could want in a TitanFall 2 cheat.

The default settings are to only aim at players, display enemy players, and use a moderate aimbot speed. Max out the FOV and Speed sliders for rage hack if you're into that. Inject after loading into a game. It works between matches without reloading, but sometimes the ESP will start showing teammates after new matches. Just eject/reinject if that happens. I recommend using the GH Injector. Press End to unload

This Titanfall 2 cheat is still a WIP, so some features disabled or need improvement:

    Aimbot aim prediction (for Kraber). Some reason the Kraber's aim doesn't line up, all other scoped weapons do. Taken out until resolved

    The overlay is kind of slow. It runs about 30 fps but it noticeably lowers your frame rate. Change the sleep() value in the DLLMain RenderLoop if it's too bad, or comment out Rerender entirely for it to be silky smooth. I need to redo it or go internal (been having trouble with DX11 in Titanfall 2)

    Aims at enemies through walls. I've been working on reversing the traceline function without much luck, need to focus more on that. The engine is based on the Source engine but it's different enough to cause headaches

    The menu uses numpad for control. Eventually want to switch to mouse control

      Numpad 8: Up
      Numpad 2: Down
      Numpad 6: Select (Or go right on a slider)
      Numpad 4: Go left on slider
      Numpad 0: Open menu

    Occasional crashes. They're rare enough to not concern me much, and I never seem to have a debugger open when it happens

    Want to implement a config file to remember your menu configuration. The default values are usually fine, but saving them would be convenient
  
