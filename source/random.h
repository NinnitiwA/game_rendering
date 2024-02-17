#pragma once
#include <iostream>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

static int Random(int Low, int High)
{
	std::uniform_int_distribution<> dist(Low, High);
	return dist(gen);
}