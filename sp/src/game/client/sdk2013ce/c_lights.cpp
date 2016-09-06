#include "cbase.h"
#include "c_lights.h"

C_EnvLight *g_pCSMEnvLight;

static ConVar csm_enabled( "csm_enabled", "1", 0, "0 = off, 1 = on, 2 = force" );

IMPLEMENT_CLIENTCLASS_DT_NOBASE( C_EnvLight, DT_CEnvLight, CEnvLight )
	RecvPropQAngles( RECVINFO( m_angSunAngles ) ),
	RecvPropVector( RECVINFO( m_vecLight ) ),
	RecvPropVector( RECVINFO( m_vecAmbient ) ),
	RecvPropBool( RECVINFO( m_bCascadedShadowMappingEnabled ) ),
END_RECV_TABLE()

C_EnvLight::C_EnvLight()
	: m_angSunAngles( vec3_angle )
	, m_vecLight( vec3_origin )
	, m_vecAmbient( vec3_origin )
	, m_bCascadedShadowMappingEnabled( false )
{
}

C_EnvLight::~C_EnvLight()
{
	if ( g_pCSMEnvLight == this )
	{
		g_pCSMEnvLight = NULL;
	}
}

void C_EnvLight::OnDataChanged( DataUpdateType_t type )
{
	BaseClass::OnDataChanged( type );

	if ( g_pCSMEnvLight == NULL ||
		m_bCascadedShadowMappingEnabled && !g_pCSMEnvLight->m_bCascadedShadowMappingEnabled ||
		m_vecLight.Length() > g_pCSMEnvLight->m_vecLight.Length() ) // If there are multiple lights, use the brightest one if CSM is forced
	{
		g_pCSMEnvLight = this;
	}
}

bool C_EnvLight::IsCascadedShadowMappingEnabled() const
{
	const int iCSMCvarEnabled = csm_enabled.GetInt();
	return m_bCascadedShadowMappingEnabled && iCSMCvarEnabled == 1 ||
		iCSMCvarEnabled == 2;
}
