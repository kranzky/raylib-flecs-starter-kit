#include <raylib.h>

#include "../components/audible.h"
#include "../components/track.h"

//==============================================================================

void play_sounds(ecs_iter_t *it)
{
  Audible *audible = ecs_column(it, Audible, 1);
  for (int i = 0; i < it->count; ++i)
  {
    if (IsAudioDeviceReady())
    {
      SetSoundVolume(*audible[i].sound, audible[i].volume);
      PlaySound(*audible[i].sound);
    }
    ecs_delete(it->world, it->entities[i]);
  }
}

//------------------------------------------------------------------------------

static inline void _update_track(Track *track, float volume, float delta)
{
  if (IsAudioDeviceReady())
  {
    SetMusicVolume(*track->music, volume);
    UpdateMusicStream(*track->music);
    track->track_time = GetMusicTimePlayed(*track->music);
  }
  else
    track->track_time += delta;
  track->state_time += delta;
}

//------------------------------------------------------------------------------

void play_music(ecs_iter_t *it)
{
  Track *track = ecs_column(it, Track, 1);
  for (int i = 0; i < it->count; ++i)
  {
    TrackState new_state = MAX_TRACKS;
    switch (track[i].state)
    {
    case TRACK_WAITING:
      if (IsAudioDeviceReady())
        PlayMusicStream(*track[i].music);
      new_state = TRACK_STARTING;
      break;
    case TRACK_STARTING:
      _update_track(&track[i], track[i].volume * track[i].state_time, it->delta_time);
      if (track[i].state_time >= 1)
        new_state = TRACK_PLAYING;
      break;
    case TRACK_PLAYING:
      _update_track(&track[i], track[i].volume, it->delta_time);
      break;
    case TRACK_MUTING:
      _update_track(&track[i], track[i].volume * (1 - track[i].state_time), it->delta_time);
      if (track[i].state_time >= 1)
        new_state = TRACK_MUTED;
      break;
    case TRACK_MUTED:
      _update_track(&track[i], 0, it->delta_time);
      break;
    case TRACK_UNMUTING:
      _update_track(&track[i], track[i].volume * track[i].state_time, it->delta_time);
      if (track[i].state_time >= 1)
        new_state = TRACK_PLAYING;
      break;
    case TRACK_STOPPING:
      _update_track(&track[i], track[i].volume * (1 - track[i].state_time), it->delta_time);
      if (track[i].state_time >= 1)
        new_state = TRACK_STOPPED;
      break;
    case TRACK_STOPPED:
      if (IsAudioDeviceReady())
        StopMusicStream(*track[i].music);
      ecs_delete(it->world, it->entities[i]);
      break;
    default:
      TraceLog(LOG_WARNING, "bad audio state");
      break;
    }
    if (new_state != MAX_TRACKS)
    {
      track[i].state = new_state;
      track[i].state_time = 0;
    }
  }
}
