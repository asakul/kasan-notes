
#ifndef BMXPCOMMANDS_H_KAV3R2XB
#define BMXPCOMMANDS_H_KAV3R2XB

namespace Bmxp
{
	enum
	{
		Signature = 0x55
	};

	enum Commands
	{
		CmdAuth = 0x02
	};

	enum AuthSubcmd
	{
		AuthPrompt = 0x01,
		AuthChallenge = 0x02,
		AuthResponse = 0x03,
		AuthResult = 0x04
	};
}

#endif /* end of include guard: BMXPCOMMANDS_H_KAV3R2XB */
