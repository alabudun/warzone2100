/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2007  Warzone Resurrection Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
/*
 * Data.c
 *
 * Data loading functions used by the framework resource module
 *
 */
#include <string.h>
#include <ctype.h>

#include "lib/framework/frame.h"
#include "lib/framework/strres.h"
//render library
#include "lib/ivis_common/piedef.h"
#include "lib/ivis_common/piestate.h"
#include "lib/ivis_common/bitimage.h"

#include "texture.h"
#include "warzoneconfig.h"
#include "lib/ivis_common/tex.h"
#include "lib/ivis_common/textdraw.h"

#include "lib/framework/frameresource.h"
#include "stats.h"
#include "structure.h"
#include "feature.h"
#include "research.h"
#include "data.h"
#include "text.h"
#include "droid.h"
#include "function.h"
#include "message.h"
#include "lib/script/script.h"
#include "scriptvals.h"
#include "display3d.h"
#include "game.h"
#include "objects.h"
#include "display.h"
#include "lib/sound/audio.h"
#include "lib/gamelib/anim.h"
#include "lib/gamelib/parser.h"
#include "levels.h"
#include "mechanics.h"
#include "display3d.h"
#include "display3ddef.h"
#include "init.h"

#include "multiplay.h"
#include "lib/netplay/netplay.h"

/**********************************************************
 *
 * Local Variables
 *
 *********************************************************/

BOOL	bTilesPCXLoaded = FALSE;

// whether a save game is currently being loaded
BOOL	saveFlag = FALSE;
extern char	aCurrResDir[255];		// Arse

UDWORD	cheatHash[CHEAT_MAXCHEAT];

void dataISpriteRelease(void *pData);

extern int scr_lineno;

/**********************************************************
 *
 * Source
 *
 *********************************************************/
static void calcCheatHash(char *pBuffer, UDWORD size, UDWORD cheat)
{
	if(!bMultiPlayer)
	{
		return;
	}

	// create the hash for that data block.
	cheatHash[cheat] =cheatHash[cheat] ^ NEThashBuffer(pBuffer,size);
	return;
}

void resetCheatHash(void)
{
	UDWORD i;
	for(i=0;i<CHEAT_MAXCHEAT;i++)
	{
		cheatHash[i] =0;
	}
}

/**********************************************************/


void dataSetSaveFlag(void)
{
	saveFlag = TRUE;
}
void dataClearSaveFlag(void)
{
	saveFlag = FALSE;
}


/* Load the body stats */
BOOL bufferSBODYLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SBODY);
	if (!loadBodyStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_BODY, numBodyStats))
	{
		return FALSE;
	}

	// set a dummy value so the release function gets called
	*ppData = (void *)1;
	return TRUE;
}

void dataReleaseStats(void *pData)
{
	freeComponentLists();
	statsShutDown();
}


/* Load the weapon stats */
BOOL bufferSWEAPONLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size, CHEAT_SWEAPON);
	if (!loadWeaponStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_WEAPON, numWeaponStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the constructor stats */
BOOL bufferSCONSTRLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SCONSTR);
	if (!loadConstructStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_CONSTRUCT, numConstructStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the ECM stats */
BOOL bufferSECMLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SECM);

	if (!loadECMStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_ECM, numECMStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Propulsion stats */
BOOL bufferSPROPLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SPROP);

	if (!loadPropulsionStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_PROPULSION, numPropulsionStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Sensor stats */
BOOL bufferSSENSORLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SSENSOR);

	if (!loadSensorStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_SENSOR, numSensorStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Repair stats */
BOOL bufferSREPAIRLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SREPAIR);

	if (!loadRepairStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_REPAIRUNIT, numRepairStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Brain stats */
BOOL bufferSBRAINLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SBRAIN);

	if (!loadBrainStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocComponentList(COMP_BRAIN, numBrainStats))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the PropulsionType stats */
BOOL bufferSPROPTYPESLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SPROPTY);

	if (!loadPropulsionTypes(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the propulsion type sound stats */
BOOL bufferSPROPSNDLoad(char *pBuffer, UDWORD size, void **ppData)
{
	if (!loadPropulsionSounds(pBuffer, size))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the SSPECABIL stats */
BOOL bufferSSPECABILLoad(char *pBuffer, UDWORD size, void **ppData)
{
	if (!loadSpecialAbility(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the STERRTABLE stats */
BOOL bufferSTERRTABLELoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_STERRT);

	if (!loadTerrainTable(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the body/propulsion IMDs stats */
BOOL bufferSBPIMDLoad(char *pBuffer, UDWORD size, void **ppData)
{
	if (!loadBodyPropulsionIMDs(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the weapon sound stats */
BOOL bufferSWEAPSNDLoad(char *pBuffer, UDWORD size, void **ppData)
{
	if (!loadWeaponSounds(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Weapon Effect modifier stats */
BOOL bufferSWEAPMODLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SWEAPMOD);

	if (!loadWeaponModifiers(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}


/* Load the Template stats */
BOOL bufferSTEMPLLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_STEMP);

	if (!loadDroidTemplates(pBuffer, size))
	{
		return FALSE;
	}


	// set a dummy value so the release function gets called
	*ppData = (void *)1;
	return TRUE;
}

// release the templates
void dataSTEMPLRelease(void *pData)
{
	//free the storage allocated to the droid templates
	droidTemplateShutDown();
}

/* Load the Template weapons stats */
BOOL bufferSTEMPWEAPLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_STEMPWEAP);

	if (!loadDroidWeapons(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Structure stats */
BOOL bufferSSTRUCTLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SSTRUCT);

	if (!loadStructureStats(pBuffer, size))
	{
		return FALSE;
	}

	if (!allocStructLists())
	{
		return FALSE;
	}

	// set a dummy value so the release function gets called
	*ppData = (void *)1;
	return TRUE;
}

// release the structure stats
void dataSSTRUCTRelease(void *pData)
{
	freeStructureLists();
	structureStatsShutDown();
}

/* Load the Structure Weapons stats */
BOOL bufferSSTRWEAPLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SSTRWEAP);

	if (!loadStructureWeapons(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Structure Functions stats */
BOOL bufferSSTRFUNCLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_STRFUNC);

	if (!loadStructureFunctions(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Structure strength modifier stats */
BOOL bufferSSTRMODLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SSTRMOD);

	if (!loadStructureStrengthModifiers(pBuffer, size))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the Feature stats */
BOOL bufferSFEATLoad(char *pBuffer, UDWORD size, void **ppData)
{

	calcCheatHash(pBuffer,size,CHEAT_SFEAT);

	if (!loadFeatureStats(pBuffer, size))
	{
		return FALSE;
	}


	// set a dummy value so the release function gets called
	*ppData = (void *)1;
	return TRUE;
}

// free the feature stats
void dataSFEATRelease(void *pData)
{
	featureStatsShutDown();
}

/* Load the Functions stats */
BOOL bufferSFUNCLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_SFUNC);

	if (!loadFunctionStats(pBuffer, size))
	{
		return FALSE;
	}

    //adjust max values of stats used in the design screen due to any possible upgrades
    adjustMaxDesignStats();

	// set a dummy value so the release function gets called
	*ppData = (void *)1;
	return TRUE;
}

// release the function stats
void dataSFUNCRelease(void *pData)
{
	FunctionShutDown();
}

// release the research stats
void dataRESCHRelease(void *pData)
{
	//free the storage allocated to the stats
	ResearchShutDown();
}

/* Load the Research stats */
BOOL bufferRESCHLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_RESCH);

    //check to see if already loaded
    if (numResearch > 0)
    {
        //release previous data before loading in the new
        dataRESCHRelease(NULL);
    }

	if (!loadResearch(pBuffer, size))
	{
		return FALSE;
	}


	/* set a dummy value so the release function gets called - the Release
    function is now called when load up the next set
	// *ppData = (void *)1;
    pass back NULL so that can load the same name file for the next campaign*/
	*ppData = NULL;
	return TRUE;
}

/* Load the research pre-requisites */
BOOL bufferRPREREQLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_RPREREQ);

	if (!loadResearchPR(pBuffer, size))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research components made redundant */
BOOL bufferRCOMPREDLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_RCOMPRED);

	if (!loadResearchArtefacts(pBuffer, size, RED_LIST))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research component results */
BOOL bufferRCOMPRESLoad(char *pBuffer, UDWORD size, void **ppData)
{

	calcCheatHash(pBuffer,size,CHEAT_RCOMPRES);

	if (!loadResearchArtefacts(pBuffer, size, RES_LIST))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research structures required */
BOOL bufferRSTRREQLoad(char *pBuffer, UDWORD size, void **ppData)
{

	calcCheatHash(pBuffer,size,CHEAT_RSTRREQ);

	if (!loadResearchStructures(pBuffer, size, REQ_LIST))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research structures made redundant */
BOOL bufferRSTRREDLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_RSTRRED);

	if (!loadResearchStructures(pBuffer, size, RED_LIST))
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research structure results */
BOOL bufferRSTRRESLoad(char *pBuffer, UDWORD size, void **ppData)
{
	calcCheatHash(pBuffer,size,CHEAT_RSTRRES);

	if (!loadResearchStructures(pBuffer, size, RES_LIST))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the research functions */
BOOL bufferRFUNCLoad(char *pBuffer, UDWORD size, void **ppData)
{

	calcCheatHash(pBuffer,size,CHEAT_RFUNC);

	if (!loadResearchFunctions(pBuffer, size))
	{
		return FALSE;
	}


	//not interested in this value
	*ppData = NULL;
	return TRUE;
}

/* Load the message viewdata */
BOOL bufferSMSGLoad(char *pBuffer, UDWORD size, void **ppData)
{
	VIEWDATA	*pViewData;

	pViewData = loadViewData(pBuffer, size);
	if (!pViewData)
	{
		return FALSE;
	}


	// set the pointer so the release function gets called with it
	*ppData = (void *)pViewData;
	return TRUE;
}


// release the message viewdata
void dataSMSGRelease(void *pData)
{
	viewDataShutDown((VIEWDATA *)pData);
}

/* Load an imd */
BOOL dataIMDBufferLoad(char *pBuffer, UDWORD size, void **ppData)
{
	iIMDShape	*psIMD;
	char *pBufferPosition = pBuffer;

	psIMD = iV_ProcessIMD( &pBufferPosition, pBufferPosition + size );
	if (psIMD == NULL) {
		debug( LOG_ERROR, "IMD load failed - %s", GetLastResourceFilename() );
		abort();
		return FALSE;
	}

	*ppData = psIMD;
	return TRUE;
}


BOOL dataIMGPAGELoad(char *pBuffer, UDWORD size, void **ppData)
{
	iTexture *psSprite = (iTexture*) MALLOC(sizeof(iTexture));
	if (!psSprite)	{
		return FALSE;
	}

	if (!pie_PNGLoadMem(pBuffer, psSprite))
	{
		debug( LOG_ERROR, "IMGPAGE load failed" );
		return FALSE;
	}

	*ppData = psSprite;

	return TRUE;
}


void dataIMGPAGERelease(void *pData)
{
	iTexture *psSprite = (iTexture*) pData;
	dataISpriteRelease(psSprite);
}

// Tertiles loader. This version for hardware renderer.
BOOL dataHWTERTILESLoad(char *pBuffer, UDWORD size, void **ppData)
{
	// tile loader.
	if (bTilesPCXLoaded)
	{
		debug( LOG_TEXTURE, "Reloading terrain tiles\n" );
		if(!pie_PNGLoadMem(pBuffer,&tilesPCX))
		{
			debug( LOG_ERROR, "HWTERTILES reload failed" );
			return FALSE;
		}
	}
	else
	{
		debug( LOG_TEXTURE, "Loading terrain tiles\n" );
		if(!pie_PNGLoadMem(pBuffer,&tilesPCX))
		{
			debug( LOG_ERROR, "HWTERTILES load failed" );
			return FALSE;
		}
	}

	getTileRadarColours();
	if (bTilesPCXLoaded)
	{
		remakeTileTexturePages(tilesPCX.width,tilesPCX.height,TILE_WIDTH, TILE_HEIGHT, tilesPCX.bmp);
	}
	else
	{
		makeTileTexturePages(tilesPCX.width,tilesPCX.height,TILE_WIDTH, TILE_HEIGHT, tilesPCX.bmp);
	}

	if (bTilesPCXLoaded)
	{
		*ppData = NULL;
	}
	else
	{
		bTilesPCXLoaded = TRUE;
		*ppData = &tilesPCX;
	}
	debug( LOG_TEXTURE, "HW Tiles loaded\n" );
	return TRUE;
}

void dataHWTERTILESRelease(void *pData)
{
	iTexture *psSprite = (iTexture*) pData;

	freeTileTextures();
	if( psSprite->bmp )
	{
		free(psSprite->bmp);
		psSprite->bmp = NULL;
	}
	// We are not allowed to free psSprite also, this would give an error on Windows: HEAP[Warzone.exe]: Invalid Address specified to RtlFreeHeap( xxx, xxx )
	bTilesPCXLoaded = FALSE;
	pie_TexShutDown();
}


BOOL dataIMGLoad(char *pBuffer, UDWORD size, void **ppData)
{
	IMAGEFILE *ImageFile;

	ImageFile = iV_LoadImageFile(pBuffer,size);
	if(ImageFile == NULL) {
		return FALSE;
	}

	*ppData = ImageFile;

	return TRUE;
}


void dataIMGRelease(void *pData)
{
	iV_FreeImageFile((IMAGEFILE*)pData);
}


/* Load a texturepage into memory */
BOOL bufferTexPageLoad(char *pBuffer, UDWORD size, void **ppData)
{
	char		texfile[255];
	char		texpage[255];
	SDWORD		i, id;

	// generate a texture page name in "page-xx" format
	strncpy(texfile, GetLastResourceFilename(), 254);
	texfile[254]=0;
	resToLower(texfile);

	//hardware
	if (strstr(texfile, "soft") != NULL) // and this is a software textpage
	{
		//so dont load it
		*ppData = NULL;
		return TRUE;
	}

	strncpy(texpage, texfile, 254);
	if (strncmp(texfile, "page-", 5) == 0)
	{
		for(i=5; i<(SDWORD)strlen(texfile); i++)
		{
			if (!isdigit(texfile[i]))
			{
				break;
			}
		}
		
		texpage[i] = 0;
	}
	SetLastResourceFilename(texpage);

	// see if this texture page has already been loaded
	if (resPresent("TEXPAGE", texpage))
	{
		// replace the old texture page with the new one
		debug(LOG_TEXTURE, "bufferTexPageLoad: replacing %s with new texture %s", texpage, texfile);
		id = pie_ReloadTexPage(texpage, pBuffer);
		ASSERT( id >=0,"pie_ReloadTexPage failed" );
		*ppData = NULL;
	}
	else
	{
		TEXTUREPAGE *NewTexturePage;
		iPalette	*psPal;
		iTexture	*psSprite;

		debug(LOG_TEXTURE, "bufferTexPageLoad: adding page %s with texture %s", texpage, texfile);

		NewTexturePage = (TEXTUREPAGE*)MALLOC(sizeof(TEXTUREPAGE));
		if (!NewTexturePage) return FALSE;

		NewTexturePage->Texture=NULL;
		NewTexturePage->Palette=NULL;

		psPal = (iPalette*)MALLOC(sizeof(iPalette));
		if (!psPal) return FALSE;

		psSprite = (iTexture*)MALLOC(sizeof(iTexture));
		if (!psSprite)
		{
			return FALSE;
		}

		if (!pie_PNGLoadMem(pBuffer, psSprite))
		{
			return FALSE;
		}

		NewTexturePage->Texture=psSprite;
		NewTexturePage->Palette=psPal;

		pie_AddBMPtoTexPages(psSprite, texpage, 1, TRUE);

		*ppData = NewTexturePage;
	}

	return TRUE;

}

/* Release an iSprite */
void dataISpriteRelease(void *pData)
{
	iTexture *psSprite = (iTexture*) pData;

	if( psSprite )
	{
		if( psSprite->bmp )
		{
			free(psSprite->bmp);
			psSprite->bmp = NULL;
		}
		FREE(psSprite);
		psSprite = NULL;
	}
}


/* Release a texPage */
void dataTexPageRelease(void *pData)
{
	TEXTUREPAGE *Tpage = (TEXTUREPAGE *) pData;

	// We need to handle null texpage data
	if (Tpage == NULL) return;

	if (Tpage->Texture != NULL)
	{
		dataISpriteRelease(Tpage->Texture);
	}
	if (Tpage->Palette != NULL)
		FREE(Tpage->Palette);

	FREE(Tpage);
}


/* Load an audio file */
BOOL dataAudioLoad(char *pBuffer, UDWORD size, void **ppData)
{
	if ( audio_Disabled() == TRUE )
	{
		*ppData = NULL;
		// No error occurred (sound is just disabled), so we return TRUE
		return TRUE;
	}
    // Load the track from a file
	*ppData = sound_LoadTrackFromBuffer( pBuffer, size );

	return *ppData != NULL;
}

void dataAudioRelease( void *pData )
{
	TRACK	*psTrack = (TRACK *) pData;

	ASSERT( PTRVALID(psTrack, sizeof(TRACK)),
			"dataAudioRelease: invalid track pointer" );

	audio_ReleaseTrack( psTrack );
}


/* Load an audio file */
BOOL dataAudioCfgLoad(char *pBuffer, UDWORD size, void **ppData)
{
	*ppData = NULL;

	if ( audio_Disabled() == FALSE &&
		 ParseResourceFile( pBuffer, size ) == FALSE )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


/* Load an anim file */
BOOL dataAnimLoad(char *pBuffer, UDWORD size, void **ppData)
{
	BASEANIM	*psAnim;

	if ( (psAnim = anim_LoadFromBuffer( pBuffer, size )) == NULL )
	{
		return FALSE;
	}

	/* copy anim for return */
	*ppData = psAnim;



	return TRUE;
}


/* Load an audio config file */
BOOL dataAnimCfgLoad(char *pBuffer, UDWORD size, void **ppData)
{
	*ppData = NULL;
	if ( ParseResourceFile( pBuffer, size ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}


void dataAnimRelease( void *pData )
{
	anim_ReleaseAnim((BASEANIM*)pData);
}

/* Load a string resource file */
BOOL dataStrResLoad(char *pBuffer, UDWORD size, void **ppData)
{
	// recreate the string resource if it was freed by a WRF release
	if (psStringRes == NULL)
	{
		if (!stringsInitialise())
		{
			return FALSE;
		}
	}

	if (!strresLoad(psStringRes, pBuffer, size))
	{
		return FALSE;
	}

	*ppData = psStringRes;
	return TRUE;
}

void dataStrResRelease(void *pData)
{
	if (psStringRes != NULL)
	{
		strresDestroy(psStringRes);
		psStringRes = NULL;
	}
}


/* Load a script file */
// All scripts, binary or otherwise are now passed through this routine
BOOL dataScriptLoad(char *pBuffer, UDWORD size, void **ppData)
{
	SCRIPT_CODE		*psProg=NULL;
	BLOCK_HEAP		*psHeap;
	BOOL			printHack = FALSE;

	calcCheatHash(pBuffer,size,CHEAT_SCRIPT);

	debug(LOG_WZ, "COMPILING SCRIPT ...%s",GetLastResourceFilename());
	// make sure the memory system uses normal malloc for a compile
	psHeap = memGetBlockHeap();
	memSetBlockHeap(NULL);

	scr_lineno = 1;

	if (!scriptCompile(pBuffer, size, &psProg, SCRIPTTYPE))		// see script.h
	{
		debug(LOG_ERROR, "Script %s did not compile", GetLastResourceFilename());
		return FALSE;
	}
	memSetBlockHeap(psHeap);

	if (printHack)
	{
		cpPrintProgram(psProg);
	}

	*ppData = psProg;

	return TRUE;
}

// Load a script variable values file
BOOL dataScriptLoadVals(char *pBuffer, UDWORD size, void **ppData)
{
	*ppData = NULL;

	calcCheatHash(pBuffer,size,CHEAT_SCRIPTVAL);

	// don't load anything if a saved game is being loaded
	if (saveFlag)
	{
		return TRUE;
	}

	debug(LOG_WZ, "Loading script data %s",GetLastResourceFilename());

	if (!scrvLoad(pBuffer, size))
	{
		debug(LOG_ERROR, "Script %s did not compile", GetLastResourceFilename());
		return FALSE;
	}

	*ppData = NULL;
	return TRUE;
}

BOOL dataSaveGameLoad(char *pFile, void **ppData)
{
	if (!stageTwoInitialise())
	{
		return FALSE;
	}

	if (!loadGameInit(pFile))
	{
		return FALSE;
	}
	if (!loadGame(pFile, !KEEPOBJECTS, FREEMEM,TRUE))
	{
		return FALSE;
	}

	if (!newMapInitialise())
	{
		return FALSE;
	}

	//not interested in this value
	*ppData = NULL;
	return TRUE;
}





// New reduced resource type ... specially for PSX
// These are statically defined in data.c
// this is also defined in frameresource.c - needs moving to a .h file
typedef struct
{
	const char *aType;			// points to the string defining the type (e.g. SCRIPT) - NULL indicates end of list
	RES_BUFFERLOAD buffLoad;		// routine to process the data for this type
	RES_FREE release;			// routine to release the data (NULL indicates none)
	void *ResourceData;			// Linked list of data - set to null initially
	UDWORD HashedType;			// hashed version of aType
} RES_TYPE_MIN;




static RES_TYPE_MIN ResourceTypes[]=
{
	{"SWEAPON", bufferSWEAPONLoad, NULL, NULL, 0},
	{"SBODY", bufferSBODYLoad, dataReleaseStats, NULL, 0},
	{"SBRAIN", bufferSBRAINLoad, NULL, NULL, 0},
	{"SPROP", bufferSPROPLoad, NULL, NULL, 0},
	{"SSENSOR", bufferSSENSORLoad, NULL, NULL, 0},
	{"SECM", bufferSECMLoad, NULL, NULL, 0},
	{"SREPAIR", bufferSREPAIRLoad, NULL, NULL, 0},
	{"SCONSTR", bufferSCONSTRLoad, NULL, NULL, 0},
	{"SPROPTYPES", bufferSPROPTYPESLoad, NULL, NULL, 0},
	{"SPROPSND", bufferSPROPSNDLoad, NULL, NULL, 0},
	{"STERRTABLE", bufferSTERRTABLELoad, NULL, NULL, 0},
	{"SSPECABIL", bufferSSPECABILLoad, NULL, NULL, 0},
	{"SBPIMD", bufferSBPIMDLoad, NULL, NULL, 0},
	{"SWEAPSND", bufferSWEAPSNDLoad, NULL, NULL, 0},
	{"SWEAPMOD", bufferSWEAPMODLoad, NULL, NULL, 0},
	{"STEMPL", bufferSTEMPLLoad, dataSTEMPLRelease, NULL, 0},	//template and associated files
	{"STEMPWEAP", bufferSTEMPWEAPLoad, NULL, NULL, 0},
	{"SSTRUCT", bufferSSTRUCTLoad, dataSSTRUCTRelease, NULL, 0},		//structure stats and associated files
	{"SSTRFUNC", bufferSSTRFUNCLoad, NULL, NULL, 0},
	{"SSTRWEAP", bufferSSTRWEAPLoad, NULL, NULL, 0},
	{"SSTRMOD", bufferSSTRMODLoad, NULL, NULL, 0},
	{"SFEAT", bufferSFEATLoad, dataSFEATRelease, NULL, 0},	//feature stats file
	{"SFUNC", bufferSFUNCLoad, dataSFUNCRelease, NULL, 0},	//function stats file
	{"RESCH", bufferRESCHLoad, dataRESCHRelease, NULL, 0},	//research stats files
	{"RPREREQ", bufferRPREREQLoad, NULL, NULL, 0},
	{"RCOMPRED", bufferRCOMPREDLoad, NULL, NULL, 0},
	{"RCOMPRES", bufferRCOMPRESLoad, NULL, NULL, 0},
	{"RSTRREQ", bufferRSTRREQLoad, NULL, NULL, 0},
	{"RSTRRED", bufferRSTRREDLoad, NULL, NULL, 0},
	{"RSTRRES", bufferRSTRRESLoad, NULL, NULL, 0},
	{"RFUNC", bufferRFUNCLoad, NULL, NULL, 0},
	{"SMSG", bufferSMSGLoad, dataSMSGRelease, NULL, 0},
	{"SCRIPT", dataScriptLoad, (RES_FREE)scriptFreeCode, NULL, 0},
	{"SCRIPTVAL", dataScriptLoadVals, NULL, NULL, 0},
	{"STR_RES", dataStrResLoad, dataStrResRelease, NULL, 0},
	{"IMGPAGE", dataIMGPAGELoad, dataIMGPAGERelease, NULL, 0},
	{"TERTILES", NULL, NULL, NULL, 0},	// This version was used when running with the software renderer.
	{"HWTERTILES", dataHWTERTILESLoad, dataHWTERTILESRelease, NULL, 0},	// freed by 3d shutdow},// Tertiles Files. This version used when running with hardware renderer.
	{"AUDIOCFG", dataAudioCfgLoad, NULL, NULL, 0},
	{"WAV", dataAudioLoad, dataAudioRelease, NULL, 0},
	{"ANI", dataAnimLoad, dataAnimRelease, NULL, 0},
	{"ANIMCFG", dataAnimCfgLoad, NULL, NULL, 0},
	{"IMG", dataIMGLoad, dataIMGRelease, NULL, 0},
	{"TEXPAGE", bufferTexPageLoad, dataTexPageRelease, NULL, 0},
	{"IMD", dataIMDBufferLoad, (RES_FREE)iV_IMDRelease, NULL, 0},


	{NULL,NULL,NULL, NULL, 0}		// indicates end of list
};


/* Pass all the data loading functions to the framework library */
BOOL dataInitLoadFuncs(void)
{
	RES_TYPE_MIN *CurrentType;
//	UDWORD	i;

	// init the cheat system;
	resetCheatHash();

	CurrentType=ResourceTypes;	// point to the first entry

	// While there are still some entries in the list
	while( CurrentType->aType != NULL )
	{
//		printf(" ==>%s\n",CurrentType->aType);	//TESTING -Q
		if(!resAddBufferLoad(CurrentType->aType,CurrentType->buffLoad,CurrentType->release))
		{
			return FALSE;	// error whilst adding a buffer load
		}
		CurrentType++;
	}

	// Now add the only file load left!
	if (!resAddFileLoad("SAVEGAME", dataSaveGameLoad, NULL))
	{
		return FALSE;
	}

	return TRUE;
}
