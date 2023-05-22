
class bootcamp {
    constructor(students) {
        this.students = students;
    }
}

class student {
    constructor(name, email) {
        this.name = name;
        this.email = email;
    }
}

function comparator( student ) {
    return student.email === "yutian@gmail.com";
}

function main() {
    // const cars = ["Tesla", "Lexus", "BMW"];
    let a = new student( "yutian", "yutian@gmail.com" );
    let b = new student( "nathan", "nathan@...");
    let c = new student( "thomas", "thomas@...");
    let students = [a,b,c];
    let bootcamp = new bootcamp( students );
    // cars.filter(comparator)
    // console.log(  );
}
window.onload = main;
