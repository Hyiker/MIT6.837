# Assignment10 - Create Your Own Assignment

## Proposal

In this assignment, I'll try to introduce some modern rendering technical flavor to the raytracer. Considering this MIT6.837 is a 2003 course, when common techniques like PBR, path tracing were far less popular than today, it could be AWESOME to implement some of them in this raytracer while keeping the overall structure.

## Features

- [x] Path tracing
  - [x] NEE(Next Event Estimation)
  - [ ] BRDF Importance Sampling
- [x] PBR materials
  - [x] (Disney Principled BRDF)
- [x] Parallel rendering(using [oneTBB](https://github.com/oneapi-src/oneTBB))

## Showcase

![scene1](./imgs/scene1.png)
perfect mirror sphere and glossy bunny(512 SPP)
