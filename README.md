# C++ REST Template
A template for scaffolding a RESTful webservice using C++.  This was a pattern I started to use a few years ago.  I have been primarily away from C++ for the last few years, so it's pretty dated, but this is a first pass at updating it.

## HTTP API Specification
So... I used a lot of [Apiary](http://apiary.io/) to generate API specs when this project template was first built.  I've removed (a.k.a. didn't generate) the Apiary artifacts for this example as I no longer intend to use them.  At the time, it was either early versions of [Swagger](https://swagger.io/) or [API Blueprint](https://apiblueprint.org/).  There was no [OpenAPI](https://www.openapis.org/) [Specification](https://github.com/OAI/OpenAPI-Specification).

Since then I've moved entirely to using OpenAPI in Go projects, and I plan to add some basic artifacts to this template. 

## REST Framework
Again, this was several years ago, but at that time my favorite option for an HTTP Request Router was the [POCO Project](https://pocoproject.org/).  This was before [proxygen](https://github.com/facebook/proxygen), which is a toolset that I believe warrants some investigating.  I'm also super curious about what the [Envoy HTTP](https://github.com/envoyproxy/envoy/tree/master/source/common/http) infrastructure has to offer.

## Reproducible Builds
One more way machine story.  This project uses [Vagrant](https://www.vagrantup.com/) as means to capture an immutable build environment for the project dependencies. Obviously, this practice predates using Docker containers as build artifacts.  Updating this project to use Docker will be the first thing that changes!
