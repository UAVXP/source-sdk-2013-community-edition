
#include "cbase.h"
#include "c_sdk2013ce_util.h"
#include "bone_setup.h"

#include "engine/ivmodelinfo.h"
#include "vcollide_parse.h"
#include "solidsetdefaults.h"

#include "vgui/ILocalize.h"
#include "tier3/tier3.h"

int ConvertPhysBoneToStudioBone( C_BaseAnimating *pEntity, int iPhysBone )
{
	int iStudioBone = -1;
	vcollide_t *pCollide = modelinfo->GetVCollide( pEntity->GetModelIndex() );

	if ( pCollide != NULL
		&& iPhysBone >= 0 )
	{
		IVPhysicsKeyParser *pParse = physcollision->VPhysicsKeyParserCreate( pCollide->pKeyValues );

		while ( !pParse->Finished() )
		{
			const char *pBlock = pParse->GetCurrentBlockName();

			// need to parse the phys solids and compare to their indices
			if ( !strcmpi( pBlock, "solid" ) )
			{
				solid_t solid;
				pParse->ParseSolid( &solid, &g_SolidSetup );

				if ( solid.index == iPhysBone )
				{
					iStudioBone = pEntity->LookupBone( solid.name );
					break;
				}
			}
			else
			{
				pParse->SkipBlock();
			}
		}

		physcollision->VPhysicsKeyParserDestroy( pParse );
	}

	return iStudioBone;
}

int BoneParentDepth( CStudioHdr *pHdr, int iBone, int iPotentialParent )
{
	if ( iBone < 0
		|| iPotentialParent < 0 )
		return -1;

	if ( iBone == iPotentialParent )
		return 0;

	int iParent = pHdr->pBone( iBone )->parent;

	if ( iParent == iPotentialParent )
		return 1;

	if ( iParent < 0 )
		return -1;

	int iRet = BoneParentDepth( pHdr, iParent, iPotentialParent );

	return ( iRet < 0 ) ? -1 : 1 + iRet;
}

int BoneParentDepth( CStudioHdr *pHdr, const char *pszBone, const char *pszPotentialParent )
{
	Assert( pHdr );

	int iBone = Studio_BoneIndexByName( pHdr, pszBone );
	int iParent = Studio_BoneIndexByName( pHdr, pszPotentialParent );

	return BoneParentDepth( pHdr, iBone, iParent );
}

bool BoneHasParent( CStudioHdr *pHdr, int iBone, int iPotentialParent )
{
	return BoneParentDepth( pHdr, iBone, iPotentialParent ) > 0;
}

bool BoneHasParent( CStudioHdr *pHdr, const char *pszBone, const char *pszPotentialParent )
{
	return BoneParentDepth( pHdr, pszBone, pszPotentialParent ) > 0;
}

bool SupportsCascadedShadows()
{
	static bool bInit = true;
	static bool bValue = false;

	if ( bInit )
	{
		bInit = false;
		bValue = g_pMaterialSystemHardwareConfig->SupportsShaderModel_3_0() &&
			g_pMaterialSystemHardwareConfig->NumVertexShaderConstants() >= 243;
	}

	return bValue;
}

const wchar_t *SafeLocalize( const char *tokenName )
{
	const wchar_t *pResult = g_pVGuiLocalize->Find( tokenName );
	if ( pResult != NULL )
	{
		return pResult;
	}
	else
	{
		static wchar_t s_wszTranslated[ 256 ];
		g_pVGuiLocalize->ConvertANSIToUnicode( tokenName, s_wszTranslated, sizeof( s_wszTranslated ) );
		return s_wszTranslated;
	}
}

SafeLocalizeInline::SafeLocalizeInline( const char *tokenName )
{
	const wchar_t *pResult = g_pVGuiLocalize->Find( tokenName );
	if ( pResult != NULL )
	{
		Q_wcsncpy( m_wszLocalized, pResult, sizeof( m_wszLocalized ) );
	}
	else
	{
		g_pVGuiLocalize->ConvertANSIToUnicode( tokenName, m_wszLocalized, sizeof( m_wszLocalized ) );
	}
}

SafeLocalizeInline::~SafeLocalizeInline()
{

}