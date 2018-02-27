# C++ REST Template
A template for scaffolding a RESTful webservice using C++.  This was a pattern I started to use a few years ago.  

# Overview
I have been away from C++ as my primary programming language for the last few years, so this is somewhat dated.  In it's current state, this is a first pass at reviving the work. I plan to update this project as a vehicle for exploring C++14/17 and developing a couple of new projects based on this template.

## Building and Running the Project
* [Building the CRT Application](docs/building.md)
* [Running the CRT Application](docs/running.md)

## HTTP API Specification
So... I used a lot of [Apiary](http://apiary.io/) to generate API specs when this project template was first built.  I've removed (a.k.a. didn't generate) the Apiary artifacts for this example as I no longer intend to use them.  At the time, it was either early versions of [Swagger](https://swagger.io/) or [API Blueprint](https://apiblueprint.org/).  There was no [OpenAPI](https://www.openapis.org/) [Specification](https://github.com/OAI/OpenAPI-Specification).

Since then I've moved entirely to using OpenAPI in Go projects, and I plan to add some basic artifacts to this template. 

## REST Framework
Again, this was several years ago, but at that time my favorite option for an HTTP Request Router was the [POCO Project](https://pocoproject.org/).  This was before [proxygen](https://github.com/facebook/proxygen), which is a toolset that I believe warrants some investigating.  I'm also super curious about what the [Envoy HTTP](https://github.com/envoyproxy/envoy/tree/master/source/common/http) infrastructure has to offer.

## Reproducible Builds
One more wayback machine story.  This project uses [Vagrant](https://www.vagrantup.com/) as means to capture an immutable build environment for the project dependencies. Obviously, this practice predates using Docker containers as build artifacts.  Updating this project to use Docker will be the first thing that changes!
