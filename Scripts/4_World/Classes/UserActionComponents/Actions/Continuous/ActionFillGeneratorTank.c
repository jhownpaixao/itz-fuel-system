modded class ActionFillGeneratorTank
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(super.ActionCondition(player, target, item))
		{
			FS_PowerGenerator generator = FS_PowerGenerator.Cast(target.GetObject());

			if (generator)
			{
				string selection = generator.GetActionComponentName( target.GetComponentIndex() );
				return selection == "fuel_tank";
			}
			else
			{
				return true;
			}
		}

		return false;
	}
};