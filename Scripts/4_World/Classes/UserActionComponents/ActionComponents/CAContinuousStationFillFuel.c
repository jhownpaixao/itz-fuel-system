class ITZ_FS_CAContinuousStationFillFuel : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_EmptySpace; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	protected PlayerBase 			m_Player;
	protected FuelStation 			m_Station;

	
	void ITZ_FS_CAContinuousStationFillFuel( float quantity_used_per_second, float time_to_progress )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	//---------------------------------------------------------------------------
	override void Setup( ActionData action_data )
	{
		m_Player = action_data.m_Player;
		
		m_TimeElpased = 0;
		m_SpentQuantity = 0;
		
		if ( !m_SpentUnits )
		{
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{
			m_SpentUnits.param1 = 0;
		}
			
		Transport vehicle = Transport.Cast(action_data.m_Target.GetObject());

		Car car = Car.Cast(vehicle);
		Boat boat = Boat.Cast(vehicle);
		
		float fuelCapacity, currentFuel;
		if (car)
		{
			fuelCapacity = car.GetFluidCapacity( CarFluid.FUEL );
			currentFuel = car.GetFluidFraction( CarFluid.FUEL );
		}
		else if (boat)
		{
			fuelCapacity = boat.GetFluidCapacity( BoatFluid.FUEL );
			currentFuel = boat.GetFluidFraction( BoatFluid.FUEL );
		}

		
		currentFuel = currentFuel * fuelCapacity;
		m_EmptySpace = (fuelCapacity - currentFuel) * 1000;

		m_Station = ITZ_FS_Module.GetModule().GetFuelPumpForTransport(vehicle);
		ITZ_FS_StationFillFuelActionData data = ITZ_FS_StationFillFuelActionData.Cast(action_data);

		m_ItemQuantity = m_Station.GetFuelQuantity(data.m_FuelType) * 1000; //convert to ml

		if ( m_EmptySpace <= m_ItemQuantity )
			m_ItemQuantity = m_EmptySpace;

	}

	//---------------------------------------------------------------------------
	override int Execute( ActionData action_data  )
	{		
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 )
		{
			return UA_FINISHED;
		}
		else
		{
			if ( m_SpentQuantity_total < m_ItemQuantity )
			{
				m_AdjustedQuantityUsedPerSecond = m_QuantityUsedPerSecond;//removed softskills 
				m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();
				
				if ( m_TimeElpased >= m_DefaultTimeStep )
				{
					CalcAndSetQuantity( action_data );
					m_TimeElpased = 0;
					//Setup(action_data);	//reset data after repeat
				}
				
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity( action_data );
				OnCompletePogress(action_data);
				return UA_FINISHED;
			}
		}
	}
	
	//---------------------------------------------------------------------------
	override int Cancel( ActionData action_data )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( action_data );
		return UA_CANCEL;
	}	
	
	//---------------------------------------------------------------------------
	override float GetProgress()
	{
		if ( m_ItemQuantity <= 0 )
			return 1;

		return -(m_SpentQuantity_total / m_ItemQuantity);
	}
	
	//---------------------------------------------------------------------------
	void CalcAndSetQuantity( ActionData action_data )
	{

		m_SpentQuantity_total += m_SpentQuantity;
	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}

		if ( GetGame().IsServer() )
		{
			ITZ_FS_StationFillFuelActionData data = ITZ_FS_StationFillFuelActionData.Cast(action_data);
			float spentQuantity = m_SpentQuantity * 0.001; //convert to liters

			Car car = Car.Cast(action_data.m_Target.GetObject());
			Boat boat = Boat.Cast(action_data.m_Target.GetObject());	
			
			if (car)
			{
				m_Station.SetFuelQuantity(data.m_FuelType, -spentQuantity );
				car.Fill( CarFluid.FUEL, spentQuantity );
			}	
			else if (boat)
			{
				m_Station.SetFuelQuantity(data.m_FuelType, -spentQuantity );
				boat.Fill( BoatFluid.FUEL, spentQuantity );
			}
		}
		
		m_SpentQuantity = 0;
	}
}