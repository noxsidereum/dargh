# `dargh`
An open source version of the Dynamic Animation Replacer (DAR) for Skyrim SE, that aims for 100% functional equivalence.

by nox sidereum

## Description

I wanted to know how Felisky's famous [Dynamic Animation Replacer](https://www.nexusmods.com/skyrimspecialedition/mods/33746), aka DAR, worked under the hood.

Unfortunately he/she does not make their source code available. So I reverse-engineered it. 

I began with the DAR v1.1.2 DLL (released 30 Dec 2021) as my reference, later updated my code to reflect changes in the DAR v1.1.3 DLL, after that was released in Jan 2023. 

This repo captures the result.

My goal was to produce readable, well-documented source code, compiling to a DLL that is 100% functionally equivalent to DAR. 

For better or worse.

Ideally the only points of difference would be: 

  1. name of DLL is different (`dargh.dll` rather than `DynamicAnimationReplacer.dll`)
  2. it writes log messages to `Skyrim Special Edition GOG/SKSE/dargh.log` rather than `Skyrim Special Edition/SKSE/DynamicAnimationReplacer.log`.

I don't know whether I have completely succeeded but I think I've come pretty close.

I haven't exhaustively tested this, but it seems to work, at least with Skyrim GOG 1.6.659. Your mileage may vary with other Skyrim versions. I *am* using the Address Library, so hopefully not.

## Credit 
I couldn't have done this without either:

* [IDA](https://hex-rays.com/ida-pro/ida-disassembler/). What an awesome disassembler.
* [CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE). Many thanks to Ryan and all the other contributors. I was hoping to fully port this to CommonLibSSE, but I will struggle to find the time now. Maybe someone else can fork this repo and take up the torch. Shouldn't be too much extra work.

## Compile Dependencies
* [SKSE64](https://github.com/ianpatt/skse64/tree/gog)
* [xbyak](https://github.com/herumi/xbyak)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Runtime Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## What next?
My driving motivation for this was more academic - to understand how DAR works, and, in so doing, continue to improve my RE skills. Achieved. I don't really want to be writing and maintaining mods, so I probably won't be revisiting this anytime soon.

If you want a DAR for "prod" rather than "dev" purposes, I recommend you continue to use Felisky's official version at https://www.nexusmods.com/skyrimspecialedition/mods/33746.

At least while that version is still being maintained.

By sharing this code I'm hoping to achieve at least three objectives:
  1. Pedagogical. Help others, who like myself, would like to learn how this stuff actually works (documentation and examples are pretty sparse).
  2. Potentially provide a platform for an improved version of DAR. E.g. you could fork this repo and add more conditions (see Conditions.cpp) for new types of conditional animations.
  3. Provide an insurance policy. If a new release of Skyrim comes out which breaks DAR, and the author of DAR is unable or unwilling to update it, others might be able to build off this code, to produce a viable, working alternative that remains compatible with the DAR-based animation mods.

Cool bananas.

## Licence
MIT Licence. 

As usual. The two main takeaways are: 

1. if you find this useful, please credit me in any derivative products.
2. don't blame me if it doesn't do what you want, or breaks something.

