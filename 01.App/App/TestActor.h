#pragma once

class TestActor final
	: public Actor
{
public:
	TestActor();

	~TestActor();

	void BeginPlay() override;

	void Update() override;

private:
	Actor* camera_;
};