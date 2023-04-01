<!DOCTYPE html>
<html>
<head>
	<title>CSClub</title>
	<style>
		table, th, td {
			border: 1px solid black;
			border-collapse: collapse;
			padding: 5px;
		}
	</style>
</head>
<body>
	<?php
		function bubbleSort(&$array)
		{
			$arraySize = count($array);
			for ($i = 0; $i < $arraySize; $i++)
			{
				for ($j = 0; $j < $arraySize - $i - 1; $j++)
				{
					if ($array[$j][2] < $array[$j+1][2])
					{
						$tempPerson = $array[$j];
						$array[$j] = $array[$j+1];
						$array[$j+1] = $tempPerson;
					}
				}
			}
		}
		
		$csclub = array
		(
			# Edit the scores for the array members to automatically sort them in decending order
			# Add people to the scoreboard using this format
			# array("name", "major", points),
			# array("other_name", "other_major", points)
		);
		
		bubbleSort($csclub);
		
		echo "<h1>CSClub Scoreboard</h1>";
		echo "<b1>A club where everything is made up</b1>";
		echo "<b1> and the points don't matter.</b1>";
		echo "<table>";
		echo "<tr><th>Name</th><th>Major</th><th>Score</th></tr>";
		foreach ($csclub as $person)
		{
			echo "<tr>";
			echo "<td>  $person[0]</td>";
			echo "<td>  $person[1]</td>";
			echo "<td>  $person[2]</td>";
			echo "</tr>";
		}
		echo "</table>";
	?>
</body>
</html>
