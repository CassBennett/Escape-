// This file lists the sounds used in the application and the location they were found/taken from

/*

+ AmbientMusic.wav
source: https://www.freesound.org/people/limetoe/sounds/256607/
renamed from: '256607__limetoe__spooky-wind.wav'
how it is used: This is used as the background music for the game and plays when the game begins
modified: n/a
stereo; 16 bit; 44100Hz.

+ BatsSound.wav
source: https://www.freesound.org/people/iankath/sounds/224022/
renamed from: '224022__iankath__fruit-bats-flying-foxes-feed-in-fig-trees.wav'
how it is used: This sound is used for a dynamic emitter that moves across the scene 
modified: changed from stereo to mono, cropped and also added reverb 
mono; 16 bit; 44100Hz.

+ CaptureNoise.wav
source: https://www.freesound.org/people/thecheeseman/sounds/51360/
renamed from: '51360__thecheeseman__camera-snap1.wav'
how it is used: This is used as the capture noise when the player tries to capture the ghost
modified: Reduced the amplification of the sound so that it is quieter
mono; 16 bit; 48000Hz.

+ CreakingFootsteps.wav
source: Two Files - https://www.freesound.org/people/tmkappelt/sounds/85687/ and https://www.freesound.org/people/Mydo1/sounds/198962/
renamed from: '198962__mydo1__footsteps-on-wood.wav' and '85687__tmkappelt__footsteps-squeeky-wood.wav'
how it is used: This sound file is used randomly when the player moves forward to imply a creaking floorboard
modified: Combined the two sound files to create a creaking sound effect on the default footsteps file.
	  Also cropped the sound files for a shorter sound, increased tempo for faster footsteps and added reverb.
mono; 16 bit; 44100Hz.

+ DefaultBreathing.wav
source: https://www.freesound.org/people/mcroce/sounds/234335/
renamed from: '234335__mcroce__breath.wav'
how it is used: This sound is used throughout the game as the breathing noise of the player
modified: Cropped the sound file to make it shorter, and amplified the sound and added reverb for the games theme.
mono; 16 bit; 48000Hz.

+ DefaultFootsteps.wav
source: https://www.freesound.org/people/Mydo1/sounds/198962/
renamed from: '198962__mydo1__footsteps-on-wood.wav'
how it is used: This sound file is used randomly when the player moves forward to show the player has moved
modified: Cropped the sound file and increased the tempo for faster footsteps, and lastly added reverb
mono; 16 bit; 44100Hz.

+ DefaultGhostNoise.wav
source: https://www.freesound.org/people/qubodup/sounds/198363/
renamed from: '198363__qubodup__ghosts.flac'
how it is used: This sound is used for the ghost when not laughing or moving, to give an extra clue to the player where he is
modified: Slightly decreased the amplification of the sound file and also cropped for a shorter sound file that can be looped
	  also changed from stereo to mono
mono; 16 bit; 48000Hz.

+ DefaultTurning.wav
source: https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used then the player turns to a new orientation
modified: The sound was cropped aswell as the tempo increased to imply player is turning with footsteps
	  The pitch was also reduced to give the sound of footsteps on wood
stereo; 16 bit; 96000Hz.

+ DoorOpen.wav
source: Two Files - https://www.freesound.org/people/joedeshon/sounds/117416/ and https://www.freesound.org/people/LG/sounds/73058/
renamed from: '117416__joedeshon__wooden-door-open.wav' and '73058__lg__door-basement-06.wav'
how it is used: This sound is used when the player successfully defeats the ghost to indicate they can now escape
modified: combined these files and inreased the tempo for a faster door open sound. Also added reverb and changed from stereo to mono
mono; 16 bit; 44100Hz.

+ DoorShut.wav
source: https://www.freesound.org/people/Zabuhailo/sounds/214081/
renamed from: '214081__zabuhailo__doorcreaking10.flac'
how it is used: This sound is used at the start of the game to signify to the player that the doors have shut
modified: increased the tempo of this sound so that the sound will play faster, in addition cropped and added reverb
stereo; 16 bit; 96000Hz.

+ FootstepsVariant1.wav
source: https://www.freesound.org/people/Mydo1/sounds/198962/
renamed from: '198962__mydo1__footsteps-on-wood.wav'
how it is used: This sound file is used randomly when the player moves forward to show the player has moved
modified: Cropped in a different way with different parts of the original sound so it is different from the default
	  footsteps sound which uses the same file. In addition, increased the tempo and added reverb
mono; 16bit; 44100Hz.

+ GhostDeath.wav
source: https://www.freesound.org/people/MaxDemianAGL/sounds/132251/
renamed from: '132251__maxdemianagl__moaning-vanishing-ghost.wav'
how it is used: This sound is used when the player defeats the ghost to show that the ghost is leaving
modified: Cropped the sound and increased the tempo while also decreasing the pitch to have a deeper voice and added reverb
	  Changed from stereo to mono. 
mono; 16 bit; 48000Hz

+ GhostIntro.wav
source: Three Files - https://www.freesound.org/people/copyc4t/sounds/219313/ and https://www.freesound.org/people/Airborne80/sounds/219181/ 
		      and https://www.freesound.org/people/DJ%20Chronos/sounds/29744/
renamed from: '219313__copyc4t__scary-monster.flac' and '219181__airborne80__cant-have-that-now-can-we-mp3.mp3' and '29744__dj-chronos__look-behind.wav'
how it is used: This sound is used at the beginning of the game to scare the player and let them know a ghost is there
modified: These sound files were combined and cropped to have an intro sequence to the game. They were changed from stereo to mono and given revert after
	  increasing their tempo.
mono; 16 bit; 44100Hz.

+ GhostLaugh1.wav
source: https://www.freesound.org/people/Airborne80/sounds/219181/ 
renamed from: '219181__airborne80__cant-have-that-now-can-we-mp3.mp3'
how it is used: This sound is randomly played when the player tries to capture the ghost but they are too far away
modified: cropped the original sound file and decreased its pitch and added reverb to fit the theme of the game
mono; 16 bit; 48000Hz

+ GhostLaugh2.wav
source: Two Files - https://www.freesound.org/people/xtrgamr/sounds/252785/ and https://www.freesound.org/people/qubodup/sounds/198363/
renamed from: '252785__xtrgamr__evil-laugh.wav' and '198363__qubodup__ghosts.flac'
how it is used: This sound is randomly played when the player tries to capture the ghost but they are too far away
modified: combined these two files and decreased their pitch and added reverb to fit the theme of the game
mono; 16 bit; 44100Hz.

+ GhostLaugh3.wav
source: Two Files - https://www.freesound.org/people/TreyHolton/sounds/252018/ and https://www.freesound.org/people/pcruzn/sounds/204819/
renamed from: '252018__treyholton__tunnel-laugh.wav' and '204819__pcruzn__ghostly-transition-1.wav'
how it is used: This sound is randomly played when the player tries to capture the ghost but they are too far away
modified: Cropped and combined these two files, changed from stereo to mono and added reverb
mono; 16 bit; 44100Hz.

+ GhostYell.wav
source: Two Files - https://www.freesound.org/people/GabrielaUPF/sounds/220287/ and https://www.freesound.org/people/pcruzn/sounds/204819/
renamed from: '220287__gabrielaupf__screaming-male-beard.wav' and '204819__pcruzn__ghostly-transition-1.wav'
how it is used: This sound is played when the player tries to capture the ghost and they are within range, damaging the ghost
modified: Cropped and combined these two files, changed the pitch to suit the voice of the ghost, changed from stereo to mono and added reverb
mono; 16bit; 48000Hz.

+ HeartBeat.wav
source: https://www.freesound.org/people/Vosvoy/sounds/178932/
renamed from: '178932__vosvoy__heartbeat-loop.wav'
how it is used: This sound is used to show the players heartbeat. The frequency and volume of the sound increase the closer the player gets to the  ghost
modified: Cropped to have a smaller sound file that could be looped
stereo; 16 bit; 48000Hz.

+ HitWallNoise.wav
source: https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used when the player collides with the wall to show they've hit a wall
modified: Cropped and changed the tempo to make a sound that sounds like the player thuds against the wall
stereo; 16 bit; 48000Hz.

+ KnockingSound.wav
source: Five Files - https://www.freesound.org/people/mark646/sounds/209220/ and https://www.freesound.org/people/altfuture/sounds/174640/ and
		     https://www.freesound.org/people/mark646/sounds/209223/ and https://www.freesound.org/people/thecluegeek/sounds/140841/ and
		     https://www.freesound.org/people/vate/sounds/268126/
renamed from: '209220__mark646__tur-klopfen-04.wav' and '174640__altfuture__door-knocking.wav' and '209223__mark646__tur-klopfen-01.wav'
	      and '140841__thecluegeek__knocking-on-a-door.wav' and '268126__vate__hard-door-banging.wav'
how it is used: This sound is used for the KNOCKING event for the ghost so that the door is knocking and draws player towards it
modified: Cropped pieces from each file and spliced them together to create different knocking noises in a sequence. Amplified the volume so that
	  door knocks are louder and also added reverb. Changed all from stereo to mono.
mono; 16 bit; 44100Hz.

+ LockedDoor.wav
source: https://www.freesound.org/people/Yap_Audio_Production/sounds/218994/
renamed from: '218994__yap-audio-production__doorlocked.wav'
how it is used: This sound is used if the player tries to interact with the door if they are still lost
modified: Cropped and tempo increased to show panic of player. Also added reverb. Changed from stereo to mono
mono; 16 bit; 44100Hz.

+ MiceSound.wav
source: Two files - https://www.youtube.com/watch?v=iEEm3XRk-dg and https://www.freesound.org/people/jhumbucker/sounds/250540/
renamed from: 'MOUSE SOUND EFFECT.wav' and '250540__jhumbucker__critters-creeping.wav'
how it is used: This sound is used as a dynamic movement of sound that moves around the screen while the game is playing
modified: Combined and cropped the two files to give a scuttering mouse sound effect, and added reverb. Changed from stereo to mono
mono; 16 bit; 44100Hz.

+ OutdoorSound.wav
source: https://www.freesound.org/people/gregswinford/sounds/70100/
renamed from: '70100__gregswinford__eerie-forest.wav'
how it is used: This sound is used with an emitter at the door of the room with a filter when the door is closed and removes the filter when it opens
		to signify to the player where the exit is
modified: Cropped to use a smaller sound file that can be looped. Also changed from stereo to mono.
mono; 16 bit; 44100Hz.

+ PainSound1.wav
source: https://www.freesound.org/people/MadamVicious/sounds/218190/
renamed from: '218190__madamvicious__girl-taking-damage.wav'
how it is used: This sound is randomly selected from the 3 pain sounds and used when the player collides with an object/wall
modified: Cropped and added reverb
stereo; 16 bit; 44100Hz.

+ PainSound2.wav
source: https://www.freesound.org/people/MadamVicious/sounds/218190/
renamed from: '218190__madamvicious__girl-taking-damage.wav'
how it is used: This sound is randomly selected from the 3 pain sounds and used when the player collides with an object/wall
modified: Cropped and added reverb
stereo; 16 bit; 44100Hz.

+ PainSound3.wav
source: https://www.freesound.org/people/MadamVicious/sounds/218190/
renamed from: '218190__madamvicious__girl-taking-damage.wav'
how it is used: This sound is randomly selected from the 3 pain sounds and used when the player collides with an object/wall
modified: Cropped and added reverb
stereo; 16 bit; 44100Hz.

+ PianoCollision.wav
source: Two Files - https://www.freesound.org/people/sim2710/sounds/262453/ and https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '262453__sim2710__piano.wav' and '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used when the player collides with the piano to show they've hit it
modified: Cropped parts of the original noise used when player bumps into wall and the piano hit noise, and added reverb
mono; 16 bit; 44100Hz.

+ PianoMusic.wav
source: "Keystone Deluge" Kevin MacLeod (incompetech.com) http://incompetech.com/music/royalty-free/index.html?genre=Silent+Film+Score&page=2
renamed from: 'Keystone Deluge.mp3'
how it is used: This sound is used for the ghost's PIANO event to have the piano playing and draw player towards it in the room
modified: Changed the pitch of the piano music lower to suit the game, cropped for a shorter sound file and added reverb. Changed from stereo to mono
	  and increased the tempo of the piece to give a more angry feeling to the piano playing
mono; 16 bit; 44100Hz.

+ PlayerOutro.wav
source: Two files - https://www.freesound.org/people/Reitanna/sounds/242909/  and https://www.freesound.org/people/Ch0cchi/sounds/15288/
renamed from: '242909__reitanna__relieved-sigh.wav' and '15288__ch0cchi__female-giggle.wav'
how it is used: This sound is used when the ghost is defeated to show the player that they are no longer in danger
modified: Combined and cropped these files to give a relieved sounding sigh and laugh. Increased the tempo. 
mono; 16 bit; 44100Hz.

+ RadioCollision.wav
source: Two Files - https://www.freesound.org/people/groovyrandomness/sounds/171257/  and https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '171257__groovyrandomness__static.wav' and '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used when the player collides with the radio to show they've hit it
modified: Cropped parts of the original noise used when player bumps into wall and the static radio noise to sound like the radio is hit, and added reverb
stereo; 16 bit; 44100Hz.

+ RadioSound.wav
source: https://www.freesound.org/people/JimiMod/sounds/215702/
renamed from: '215702__jimimod__abstract-radio-communications.aiff'
how it is used: This sound is used for the ghost event RADIO to play the radio and draw the player towards it
modified: Cropped original so it is only a short loopable sound file, added reverb and changed from stereo to mono.
mono; 16 bit; 44100Hz.

+ RoomIntro.wav
source: 5 Files - Door Slam - https://www.freesound.org/people/Zabuhailo/sounds/214081/
		- Gasping - https://www.freesound.org/people/tcrocker68/sounds/235586/ 
		- Flash light clicks - https://www.freesound.org/people/mshahen/sounds/271109/
		- Bulb Smash - https://www.freesound.org/people/sandyrb/sounds/148074/
		- 'Oh God' - https://www.freesound.org/people/Reitanna/sounds/241534/
renamed from: '214081__zabuhailo__doorcreaking10.flac' and '235586__tcrocker68__girl-gasp-heavy-breathing.wav' and '271109__mshahen__flashlight-on-off-clicks.wav'
		and '148074__sandyrb__lightbulb-smash-002.wav' and '241534__reitanna__scared-oh-god.wav'
how it is used: This sound is used as the intro sound to the game and is played as soon as the game runs. It serves as an introduction for the player to their
		situation.
modified: Combined these sounds in order to create a scripted sequence of events: Doors slam shut, player tries to turn flashlight on, flashlight breaks, and they
	  exclaim 'oh god' at what is happening. Cropped and combined the sounds to fit in this way, and changed from stereo to mono.
mono; 16 bit; 96000Hz.

+ TableCollision.wav
source: Two files - https://www.freesound.org/people/squareal/sounds/237382/ and https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '237382__squareal__cutlery-grabbing-01.wav' and '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used when the player collides with the table to show they've hit it
modified: Cropped parts of the original noise used when player bumps into wall and the cutlery noise to sound as if a table is hit, and added reverb
mono; 16 bit; 44100Hz.

+ Typewriter.wav
source: https://www.freesound.org/people/TOC1/sounds/244414/
renamed from: '244414__toc1__typewriter.aiff'
how it is used: This sound is used for the ghost event TYPEWRITER to play the typewriter sound and draw player closer
modified: Cropped the sound to a shorter sound file and amplified the sound and added reverb. Also changed from stereo to mono.
mono; 16 bit; 44100Hz.

+ TypewriterCollision.wav
source: Two Files - https://www.freesound.org/people/TOC1/sounds/244414/ and https://www.freesound.org/people/zerolagtime/sounds/245033/
renamed from: '244414__toc1__typewriter.aiff' and '245033__zerolagtime__thuds-on-window.wav'
how it is used: This sound is used when the player collides with the typewriter to show they've hit it
modified: Cropped parts of the original noise used when player bumps into wall and the typewriter bell noise to sound as if it is hit, and added reverb
stereo; 16 bit; 44100Hz.


=== end of references.

*/