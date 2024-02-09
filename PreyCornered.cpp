#include "PreyCornered.h"

//
// Check whether the prey should be considered as "cornered" (killed)
bool PreyCornered::Success(void)
{
	double d;
	bool result = false;
	int count;

	// Go through each prey
	for (int ny = 0; ny < NPREY; ++ny)
	{
		// See how many predators surround this prey agent
		count = 0;
		for (int nd = 0; nd < NPRED; ++nd)
		{
			if (preds[nd]->getTarget() == ny)
			{
				d = prey[ny]->getPosition().getDistance(preds[nd]->getPosition());
				if (d < margin)
					++count;
			}
		}

		// If there are enough of them, increment the number of turns for which that is the case, otherwise reset it to zero
		if (count >= minpredators)
			++dur[ny];
		else
			dur[ny] = 0;

		// If both conditions are met, this animal is DEAD
		if (dur[ny] == minduration)
		{
			result = true;
			victim = ny;

			break;
		}
	}

	return result;
}