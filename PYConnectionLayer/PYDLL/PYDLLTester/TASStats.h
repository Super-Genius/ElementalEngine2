#pragma once

// This only needs to be called once per game
bool TASSetupStatsConnection();

// This function puts the given stats onto the queue for delivery and returns a UINT32 handle to the request
// This function does NOT block.  It returns a 0 if it failed to put the request on the queue.
// otherwise it returns a UINT32 that can be used to get the status of the sent stats
// Since stats are associated with a user, the username and password is included
// nonzero return value means the request was successfully placed on the queue
// it does NOT mean that it actually arrived.
unsigned long SendTASGameStats(const char * const phatyaffle_username, const char * const phatyaffle_password,
							   unsigned long score,float hit_miss_ratio);

// you can use this to check if the stat is done
// done meaning failed, cancelled, whatever
bool TASIsStatSendComplete(const unsigned long requestID);