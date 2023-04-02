<!DOCTYPE html>
<html>
<head>
	<title>CSClub Scores</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="style.css">
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
		
	<h1>CS Club rocks!</h1>
	<p>Welcome to our webpage! We are excited to share with you all the amazing things happening in our CS Club. From coding challenges to hackathons, our club is the place to be for anyone interested in computer science. Our members are passionate about technology and eager to learn new skills. Join us and see for yourself why CS Club rocks!</p>
	
	<p>Thank you for visiting our webpage. We hope to see you at one of our events soon!</p>

</body>
</html>
