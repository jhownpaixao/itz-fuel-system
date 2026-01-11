class ITZ_FS_CAContinuousStationFillFuelBottle : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_EmptySpace; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_ItemMaxQuantity; //basically free capacity
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	protected PlayerBase 			m_Player;
	protected int					m_FuelType;
	protected FuelStation 			m_Station;

	void ITZ_FS_CAContinuousStationFillFuelBottle( float quantity_used_per_second, float time_to_progress )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	override void Setup( ActionData action_data )
	{
		m_TimeElpased = 0;
		
		if ( !m_SpentUnits )
			m_SpentUnits = new Param1<float>(0);
		else
			m_SpentUnits.param1 = 0;

		ITZ_FS_StationFillFuelBottleActionData data = ITZ_FS_StationFillFuelBottleActionData.Cast(action_data);
		m_FuelType = data.m_FuelType;

		m_Station = FuelStation.Cast(action_data.m_Target.GetObject());
		float fuelQuantity = m_Station.GetFuelQuantity(m_FuelType) * 1000;
		float currentItemAvailableSpace = action_data.m_MainItem.GetQuantityMax() - action_data.m_MainItem.GetQuantity();

		if (currentItemAvailableSpace > fuelQuantity)
			m_ItemMaxQuantity = fuelQuantity;
		else
			m_ItemMaxQuantity = currentItemAvailableSpace;

		m_ItemQuantity = m_ItemMaxQuantity;		
	}
	
	override int Execute( ActionData action_data )
	{		
		if ( !action_data.m_Player)
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 || !m_Station.HasEnergy() || !m_Station.HasFuel(m_FuelType) )
		{			
			return UA_FINISHED;
		}
		else
		{
			if (m_SpentQuantity_total < m_ItemQuantity)
			{
				m_AdjustedQuantityUsedPerSecond = m_QuantityUsedPerSecond;//removed softskills 
				m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();
				
				if (m_TimeElpased >= m_DefaultTimeStep)
				{
					CalcAndSetQuantity(action_data);
					m_TimeElpased = 0;
				}
				
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(action_data);
				OnCompletePogress(action_data);
				return UA_FINISHED;
			}
		}
	}
	
	override int Cancel( ActionData action_data )
	{
		if ( !action_data.m_Player || !action_data.m_MainItem )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( action_data );
		return UA_CANCEL;
	}
	
	override float GetProgress()
	{	
		return -(m_SpentQuantity_total / m_ItemMaxQuantity);
	}
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity(ActionData action_data)
	{
		m_SpentQuantity_total += m_SpentQuantity;
		
		if (GetGame().IsServer())
		{
			if (m_SpentUnits)
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}
			Liquid.FillContainer(action_data.m_MainItem, m_FuelType, m_SpentQuantity);
			m_Station.SetFuelQuantity(m_FuelType, -(m_SpentQuantity / 1000));
		}

		m_SpentQuantity = 0;
	}
}