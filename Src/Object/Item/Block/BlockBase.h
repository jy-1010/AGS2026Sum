#pragma once
#include "../../ObjectBase.h"

class BlockBase : public ObjectBase
{
public:

	BlockBase() = default;
	virtual ~BlockBase() = default;
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void UIDraw(void) override;
protected:

private:
};

