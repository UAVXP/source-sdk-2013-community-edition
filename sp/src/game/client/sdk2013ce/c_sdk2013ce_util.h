#ifndef C_GSTRING_UTIL_H
#define C_GSTRING_UTIL_H


extern int ConvertPhysBoneToStudioBone( C_BaseAnimating *pEntity, int iPhysBone );

extern int BoneParentDepth( CStudioHdr *pHdr, int iBone, int iPotentialParent );
extern int BoneParentDepth( CStudioHdr *pHdr, const char *pszBone, const char *pszPotentialParent );

extern bool BoneHasParent( CStudioHdr *pHdr, int iBone, int iPotentialParent );
extern bool BoneHasParent( CStudioHdr *pHdr, const char *pszBone, const char *pszPotentialParent );

extern bool SupportsCascadedShadows();

extern const wchar_t *SafeLocalize( const char *tokenName );

class SafeLocalizeInline
{
public:
	SafeLocalizeInline( const char *tokenName );
	~SafeLocalizeInline();

	operator const wchar_t *() const
	{
		return m_wszLocalized;
	}

	const wchar_t *Get()
	{
		return m_wszLocalized;
	}

private:
	wchar_t m_wszLocalized[ 256 ];
};

#endif